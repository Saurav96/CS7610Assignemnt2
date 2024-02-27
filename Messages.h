#ifndef __MESSAGES_H__
#define __MESSAGES_H__

#include <string>

class MapOp {
private:
    int opcode; 
    int arg1;   
    int arg2;   

public:
    MapOp();
	void operator = (const MapOp &order) {
		opcode = order.opcode;
		arg1 = order.arg1;
		arg2 = order.arg2;
	}
    void SetOpcode(int opcode);
    void SetArg1(int arg1);
    void SetArg2(int arg2);
    int GetOpcode() const;
    int GetArg1() const;
    int GetArg2() const;

    void Marshal(char* buffer, int &offset) ;
    void Unmarshal(char* buffer, int &offset);

	void print();
};

class ServerReplication {
private:
    int last_index;
    int commited_index;
    int primary_id;
    int factory_id;

public:
    ServerReplication();
	void operator = (const ServerReplication &order) {
		last_index = order.last_index;
		commited_index = order.commited_index;
		primary_id = order.primary_id;
		factory_id = order.factory_id;
	}
    ServerReplication(int last_index, int commited_index, int primary_id, int factory_id);

    void Marshal(char* buffer, int &offset);
    void Unmarshal(char* buffer, int &offset);

    // Getters and Setters
    void setLastIndex(int lastIndex);
    void setCommitedIndex(int commitedIndex);
    void setPrimaryId(int primaryId);
    void setFactoryId(int factoryId);

    int getLastIndex() const;
    int getCommitedIndex() const;
    int getPrimaryId() const;
    int getFactoryId() const;

	void print();
};

class CustomerRequests {
private:
	int customer_id;
	int order_number;
	int request_type;
	int type_of_request;
	MapOp obj;
	ServerReplication replication;

public:
	CustomerRequests();
	void operator = (const CustomerRequests &order) {
		customer_id = order.customer_id;
		order_number = order.order_number;
		request_type = order.request_type;
		type_of_request = order.type_of_request;
		obj = order.obj;
		replication = order.replication;
	}
	void SetOrder(int cid, int order_num, int type, int type_of_request, MapOp obj, ServerReplication replication);
	void setMapObj(MapOp obj);
	void setCustomerId();
	int GetCustomerId();
	int GetOrderNumber();
	int GetRequestType();
	int GetTypeOfRequest();
	MapOp GetMapObj();
	ServerReplication GetReplication();

	int Size();

	void Marshal(char *buffer);
	void Unmarshal(char *buffer);

	bool IsValid();

	void Print();
};


class LaptopInfo {
private:
	int customer_id;
	int order_number;
	int request_type;
	int engineer_id;
	int admin_id;

public:
	LaptopInfo();
	void operator = (const LaptopInfo &info) {
		customer_id = info.customer_id;
		order_number = info.order_number;
		request_type = info.request_type;
		engineer_id = info.engineer_id;
		admin_id = info.admin_id;
	}
	void SetInfo(int cid, int order_num, int type, int engid, int expid);
	void CopyOrder(CustomerRequests order);
	void SetEngineerId(int id);
	void SetAdminId(int id);
	int GetCustomerId();
	int GetOrderNumber();
	int GetRequestType();
	int GetEngineerId();
	int GetAdminId();

	int Size();

	void Marshal(char *buffer);
	void Unmarshal(char *buffer);

	bool IsValid();

	void Print();
};

class Customer_Record {
	private:
	int customer_id;
	int last_order;

	public:
	Customer_Record();
	void operator = (const Customer_Record &record){
		customer_id = record.customer_id;
		last_order = record.last_order;
	}

	void SetInfo(int customer_id, int last_order);
	void SetCustomerId(int id);
	void SetLastOrder(int id);
	int GetCustomerId();
	int GetLastOrder(); 
	int Size();

	void Marshal(char *buffer);
	void Unmarshal(char *buffer);

	bool IsValid();

};




#endif // #ifndef __MESSAGES_H__

