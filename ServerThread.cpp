#include <iostream>
#include <memory>
#include "ServerThread.h"
LaptopInfo LaptopFactory::CreateRegularLaptop(CustomerRequests order, int engineer_id){
	LaptopInfo laptop;
	laptop.CopyOrder(order);
	laptop.SetEngineerId(engineer_id);
	MapOp obj;
	return laptop;
}

LaptopInfo LaptopFactory::updateRecord(CustomerRequests record, int engineer_id){
	LaptopInfo laptop;
	laptop.CopyOrder(record);
	laptop.SetEngineerId(engineer_id);
	std::promise<LaptopInfo> prom;
	std::future<LaptopInfo> fut = prom.get_future();

	std::unique_ptr<AdminRequest> req = 
		std::unique_ptr<AdminRequest>(new AdminRequest);

	req->laptop = laptop;
	req->prom = std::move(prom);

	admin_lock.lock();
	adminQueue.push(std::move(req));
	admin_cv.notify_all();
	admin_lock.unlock();
	laptop = fut.get();
	return laptop;

}

Customer_Record LaptopFactory::FetchCustomerRecord(int customer_id){
	Customer_Record record;
	if(_customerRecords.find(customer_id) == _customerRecords.end()){
		record.SetInfo(customer_id, -1);
	}
	else{
		auto value = _customerRecords.find(customer_id);
		record.SetInfo(customer_id, value->second);
	}
	return record;
}

void LaptopFactory::createLogRecord(LaptopInfo info){
	
	obj.SetOpcode(1);
	obj.SetArg1(info.GetCustomerId());
	obj.SetArg2(info.GetOrderNumber());
	
}

void LaptopFactory::updateCustomerRecordOnLog(MapOp obj){
	std::lock_guard<std::mutex> vector_guard(_vector_mutex);
	_stateMachineLog.emplace_back(obj);
	replication.setLastIndex(_stateMachineLog.size()-1);
	


}
void LaptopFactory::updateCustomerRecordOnMap(MapOp obj){
	std::lock_guard<std::mutex> map_guard(_map_mutex);
	_customerRecords[obj.GetArg1()] = obj.GetArg2();
	replication.setCommitedIndex(_stateMachineLog.size()-1);
}

void LaptopFactory::
EngineerThread(std::unique_ptr<ServerSocket> socket, int id) {
	int engineer_id = id;
	int laptop_type;
	CustomerRequests request;
	ServerStub stub;
	stub.Init(std::move(socket));
	while (true) {
		request = stub.ReceiveRequest();
		//std::cout<<"request Received " << request.GetReplication().getFactoryId()<<std::endl;
		//std::cout<<"Customer ID" <<request.GetCustomerId()<<std::endl;
		if (!request.IsValid()) {
			break;	
		}

		if(isReplication(request)){
			processReplicationRequest(request);
		}
		else{
		laptop_type = request.GetRequestType();
		switch (laptop_type) {
			case 1: {
				LaptopInfo laptop;
				laptop = updateRecord(request, engineer_id);
				stub.ShipLaptop(laptop);
				break;
			}
			case 2: {
				Customer_Record record;
				int customer_id = request.GetCustomerId();
				record = FetchCustomerRecord(customer_id);
				stub.ReturnRecord(record);
				break;
			}
			default:
				std::cout << "Undefined laptop type: "
					<< laptop_type << std::endl;
			}
		}
		
	}
}

void LaptopFactory::AdminThread(int id) {
	std::unique_lock<std::mutex> ul(admin_lock, std::defer_lock);
	while (true) {
		ul.lock();

		if (adminQueue.empty()) {
			admin_cv.wait(ul, [this]{ return !adminQueue.empty(); });
		}
		auto req = std::move(adminQueue.front());
		adminQueue.pop();
		ul.unlock();
		int getCustomerId = req->laptop.GetCustomerId();
		int orderNumber = req->laptop.GetOrderNumber();
		int requestType = req->laptop.GetRequestType();
		obj.SetArg1(getCustomerId);
		obj.SetArg2(orderNumber);
		createLogRecord(req->laptop);		

		//Updates the Primary
		if((replication.getPrimaryId() == -1) || (replication.getPrimaryId() != unique_id))
		{
				if(_stateMachineLog.size() == 0){
					replication.setPrimaryId(unique_id);
				}
				else{
					MapOp tempObj = _stateMachineLog[replication.getLastIndex()];
					std::lock_guard<std::mutex> map_guard(_map_mutex);
					_customerRecords[tempObj.GetArg1()] = tempObj.GetArg2();
					replication.setCommitedIndex(replication.getLastIndex());
					replication.setPrimaryId(unique_id);
				}
		}
	
		updateCustomerRecordOnLog(obj);
		CustomerRequests requestPeer;
		ServerReplication replicationPeer;
		MapOp objPeer;
		//Iterate over all the peers
		for(PeerInfo ifa:ifaAddresses){
			ServerClientStub stub;
			std::string ip = ifa.getIP();
			int port = ifa.getPort();
			int peerID = ifa.getPeerId();
			if(peerID == unique_id){
				continue;
			}
			replicationPeer.setFactoryId(peerID);
			replicationPeer.setPrimaryId(unique_id);
			std::cout<<_stateMachineLog.size()-1<<std::endl;
			replicationPeer.setLastIndex(_stateMachineLog.size()-1);
			objPeer = obj;
			requestPeer.SetOrder(peerID, orderNumber,  requestType, 1, objPeer, replicationPeer );
			if(stub.Init(ip, port))
			{
				if(stub.sendUpdate(requestPeer))
				{
					//std::cout<<"Successful"<<std::endl;
				}
			}
		}
		
		updateCustomerRecordOnMap(obj);
		req->laptop.SetAdminId(id);
		req->prom.set_value(req->laptop);

		
	}
}


// 1 for Replication
// 0 for Client Request
bool LaptopFactory::isReplication(CustomerRequests request){
	return request.GetTypeOfRequest() == 1;
}

void LaptopFactory::processReplicationRequest(CustomerRequests request){


	if(replication.getFactoryId() == -1){
		replication.setFactoryId(unique_id);
	}

	if(replication.getPrimaryId()  == -1 ){
		replication.setPrimaryId(request.GetReplication().getPrimaryId());
	}
	
	if(replication.getPrimaryId() != -1 && replication.getPrimaryId() != request.GetReplication().getPrimaryId()){
		std::cout<<"coming insde here"<<std::endl;
		MapOp tempObj = _stateMachineLog[replication.getLastIndex()];
		std::lock_guard<std::mutex> map_guard(_map_mutex);
		_customerRecords[tempObj.GetArg1()] = tempObj.GetArg2();
		replication.setCommitedIndex(replication.getLastIndex());
		replication.setPrimaryId(request.GetReplication().getPrimaryId());
	}
	std::cout<<replication.getCommitedIndex()<<" commited"<<replication.getLastIndex()<<std::endl;
	if(replication.getLastIndex() != -1) {
		MapOp tempObj = _stateMachineLog[replication.getLastIndex()];
		std::lock_guard<std::mutex> map_guard(_map_mutex);
		_customerRecords[tempObj.GetArg1()] = tempObj.GetArg2();
		replication.setCommitedIndex(replication.getCommitedIndex());
	}
	replication.setLastIndex(request.GetReplication().getLastIndex());
	replication.setCommitedIndex(request.GetReplication().getLastIndex()-1);
	MapOp obj1 = request.GetMapObj();
	std::lock_guard<std::mutex> vector_guard(_vector_mutex);
	_stateMachineLog.emplace_back(obj1);

}