#include "ClientThread.h"
#include "Messages.h"
#include <cctype>
#include <iostream>

ClientThreadClass::ClientThreadClass() {}

void ClientThreadClass::
ThreadBody(std::string ip, int port, int id, int orders, int type) {
	customer_id = id;
	request_type = type;
	num_orders = orders;
	if (!stub.Init(ip, port)) {
		std::cout << "Thread " << customer_id << " failed to connect" << std::endl;
		return;
	}
	if (request_type == 1) { 

		for(int i=1;i<=num_orders;++i){
            CustomerRequests request;
            LaptopInfo laptop;
			MapOp obj;
			ServerReplication ServerReplication;
            request.SetOrder(customer_id, i , request_type, 0, obj, ServerReplication); 
            laptop = stub.OrderLaptop(request);
            if (!laptop.IsValid()) {
                std::cout << "Invalid laptop for customer " << customer_id << std::endl;
            }
		}
    }
	else if(request_type == 2){
			CustomerRequests request;
			MapOp obj;
			ServerReplication replication;
        	request.SetOrder(customer_id, -1, request_type,0, obj, replication);
        	Customer_Record record = stub.ReadRecord(request); 
			if(!record.IsValid()){
				std::cout<<"Invalid Record for Customer" <<customer_id <<std::endl;
			}
			std::cout<<"For Customer "<<record.GetCustomerId()<<" Value is "<< record.GetLastOrder();
	}
	else if(request_type == 3){
		for(int i=id;i<=orders;++i){
			CustomerRequests request;
			MapOp obj;
			ServerReplication replication;
			request.SetOrder(i,-1,request_type-1,0, obj, replication);
			Customer_Record record = stub.ReadRecord(request);
			if(!record.IsValid()){
				std::cout<<"Invalid Record for Customer" <<customer_id <<std::endl;
			}
			if(record.GetLastOrder() == -1){
				continue;
			}
			std::cout<<record.GetCustomerId()<<"\t"<<record.GetLastOrder()<<std::endl;
		}

	}
	else {
		std::cout<<"Request Type Not valid"<<std::endl;
	}
		

		
}

ClientTimer ClientThreadClass::GetTimer() {
	return timer;	
}

