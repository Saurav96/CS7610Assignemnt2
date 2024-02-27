#include "ClientStub.h"

ClientStub::ClientStub() {}

int ClientStub::Init(std::string ip, int port) {
	try{
		return socket.Init(ip, port);
	}
	catch (const std::runtime_error& e){
		std::cout<<"Oops!! Server is down!!"<<std::endl;
		return 0;
	}	
}

LaptopInfo ClientStub::OrderLaptop(CustomerRequests request) {
	LaptopInfo info;
	char buffer[64];
	int size;
	request.Marshal(buffer);
	size = request.Size();
	if (socket.Send(buffer, size, 0)) {
		size = info.Size();
		if (socket.Recv(buffer, size, 0)) {
			info.Unmarshal(buffer);
		} 
	}
	return info;

}

Customer_Record ClientStub::ReadRecord(CustomerRequests request){
	Customer_Record record;
	char buffer[64];
	int size;
	request.Marshal(buffer);
	size = request.Size();
	if(socket.Send(buffer, size,0)) {
		size = record.Size();
		if(socket.Recv(buffer, size, 0)) {
			record.Unmarshal(buffer);
		}
	}
	return record;
}

