#ifndef __SERVERTHREAD_H__
#define __SERVERTHREAD_H__

#include <condition_variable>
#include <future>
#include <queue>
#include <thread>
#include <map>
#include "ServerSocket.h"
#include "ServerClientStub.h"
#include "ServerPeerInfo.h"
#include "ServerStub.h"

struct AdminRequest {
	LaptopInfo laptop;
	std::promise<LaptopInfo> prom;
};

class LaptopFactory {
private:
	std::queue<std::unique_ptr<AdminRequest>> adminQueue;
	std::mutex admin_lock;
	std::condition_variable admin_cv;
	std::mutex _vector_mutex;
	std::mutex _map_mutex;
	std::map<int, int> _customerRecords;
	std::vector<MapOp> _stateMachineLog;
	LaptopInfo CreateRegularLaptop(CustomerRequests order, int engineer_id);
	Customer_Record FetchCustomerRecord(int customer_id);
	void updateCustomerRecordOnLog(MapOp obj);
	void updateCustomerRecordOnMap(MapOp obj);
	LaptopInfo updateRecord(CustomerRequests order, int id);
	void createLogRecord(LaptopInfo info);
	bool isReplication(CustomerRequests request);
	void processReplicationRequest(CustomerRequests request);
	void updateBackup();
	void sendUpdationToBackups(CustomerRequests request);

	
public:
	void EngineerThread(std::unique_ptr<ServerSocket> socket, int id);
	void AdminThread(int id);
	ServerReplication replication;
	MapOp obj;
	
};



#endif // end of #ifndef __SERVERTHREAD_H__

