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
	try
	{
		if (!stub.Init(ip, port)) {
			return;
		}
	}
	catch (const std::runtime_error& e)
	{
		std::cerr << " Oops!! Server is  Down" << e.what() << std::endl;
		return;
	}

	//Request type 1 is to update the records in System
	if (request_type == 1) { 

		for(int i=1;i<=num_orders;++i){
            CustomerRequests request;
            LaptopInfo laptop;
			MapOp obj;
			ServerReplication ServerReplication;
            request.SetOrder(customer_id, i , request_type, 0, obj, ServerReplication); 

			timer.Start();
            laptop = stub.OrderLaptop(request);
			timer.EndAndMerge();

            if (!laptop.IsValid()) {
                std::cout << "Invalid laptop for customer " << customer_id << std::endl;
            }
		}
    }
	//Request Type 2 is to get a particular value for that request
	else if(request_type == 2){
			CustomerRequests request;
			MapOp obj;
			ServerReplication replication;
        	request.SetOrder(num_orders, -1, request_type,0, obj, replication);
        	Customer_Record record = stub.ReadRecord(request); 
			if(!record.IsValid()){
				std::cout<<"Invalid Record for Customer" <<customer_id <<std::endl;
			}
			std::cout<<"The value of customer "<< record.GetCustomerId() <<" is " <<record.GetLastOrder()<<std::endl;
	}

	
	//Request Type 3 is just printing
	else if(request_type == 3){
		for(int i=0;i<=num_orders;++i){
			
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

