#include <iostream>
#include <memory>
#include "ServerThread.h"
/*
Craeting Regualar Laptops,

*/
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

//Fetch the customer data, if not found, set it to -1
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

//Create a new Log record
void LaptopFactory::createLogRecord(LaptopInfo info){
	
	obj.SetOpcode(1);
	obj.SetArg1(info.GetCustomerId());
	obj.SetArg2(info.GetOrderNumber());
	
}
//Updates the log
void LaptopFactory::updateCustomerRecordOnLog(MapOp obj){
	std::lock_guard<std::mutex> vector_guard(_vector_mutex);
	_stateMachineLog.emplace_back(obj);
	replication.setLastIndex(_stateMachineLog.size()-1);	
}

//updates the Map Structure
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
		// Gets the Customer details
		int getCustomerId = req->laptop.GetCustomerId();
		int orderNumber = req->laptop.GetOrderNumber();
		int requestType = req->laptop.GetRequestType();
		//Create a new mapOb Type
		obj.SetArg1(getCustomerId);
		obj.SetArg2(orderNumber);
		createLogRecord(req->laptop);		

		//Updates the Primary
		if((replication.getPrimaryId() == -1) || (replication.getPrimaryId() != unique_id))
		{
				//if its a first request, set primary
				if(_stateMachineLog.size() == 0){
					replication.setPrimaryId(unique_id);
				}
				// If the primary is updated by client, update the logs and sets a new primary
				else{
					MapOp tempObj = _stateMachineLog[replication.getLastIndex()];
					std::lock_guard<std::mutex> map_guard(_map_mutex);
					_customerRecords[tempObj.GetArg1()] = tempObj.GetArg2();
					replication.setCommitedIndex(replication.getLastIndex());
					replication.setPrimaryId(unique_id);
				}
		}
	
		//update the Log file
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
			//Set Factory and primary Id for replicas
			replicationPeer.setFactoryId(peerID);
			replicationPeer.setPrimaryId(unique_id);
			replicationPeer.setLastIndex(_stateMachineLog.size()-1);
			objPeer = obj;
			//Create a replica request
			requestPeer.SetOrder(peerID, orderNumber,  requestType, 1, objPeer, replicationPeer );
			try {
				if(stub.Init(ip, port))
				{	

					if(stub.sendUpdate(requestPeer))
					{
					//std::cout<<"Successful"<<std::endl;
					}
				}
			}
			catch (const std::runtime_error& e)
			{
				return;
			}
			
			
		}
		//Update the Map
		updateCustomerRecordOnMap(obj);
		//Set admin ID
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

	//If Ids are not set, set it
	if(replication.getFactoryId() == -1){
		replication.setFactoryId(unique_id);
	}

	//Set the PrimaryId for Replicas
	if(replication.getPrimaryId()  == -1 ){
		replication.setPrimaryId(request.GetReplication().getPrimaryId());
	}
	
	//If its a new Call where primary is not set, set it.
	if(replication.getPrimaryId() != -1 && replication.getPrimaryId() != request.GetReplication().getPrimaryId()){
		MapOp tempObj = _stateMachineLog[replication.getLastIndex()];
		std::lock_guard<std::mutex> map_guard(_map_mutex);
		_customerRecords[tempObj.GetArg1()] = tempObj.GetArg2();
		replication.setCommitedIndex(replication.getLastIndex());
		replication.setPrimaryId(request.GetReplication().getPrimaryId());
	}

	//Append the getLastIndex and GetCommitedIndex for all request other than first for replicas
	if(replication.getLastIndex() != -1) {
		MapOp tempObj = _stateMachineLog[replication.getLastIndex()];
		std::lock_guard<std::mutex> map_guard(_map_mutex);
		_customerRecords[tempObj.GetArg1()] = tempObj.GetArg2();
		replication.setCommitedIndex(replication.getCommitedIndex());
	}

	//Updates logs for Replicas
	replication.setLastIndex(request.GetReplication().getLastIndex());
	replication.setCommitedIndex(request.GetReplication().getLastIndex()-1);
	MapOp obj1 = request.GetMapObj();
	std::lock_guard<std::mutex> vector_guard(_vector_mutex);
	_stateMachineLog.emplace_back(obj1);

}