#include "ServerStub.h"

ServerStub::ServerStub() {}

void ServerStub::Init(std::unique_ptr<ServerSocket> socket) {
	this->socket = std::move(socket);
}

CustomerRequests ServerStub::ReceiveRequest() {
	char buffer[64];
	CustomerRequests request;
	if (socket->Recv(buffer, request.Size(), 0)) {
		request.Unmarshal(buffer);
	}
	return request;
	
}

int ServerStub::ShipLaptop(LaptopInfo info) {
	char buffer[64];
	info.Marshal(buffer);
	return socket->Send(buffer, info.Size(), 0);
}

int ServerStub::ReturnRecord(Customer_Record record) {
    char buffer[64]; 
    record.Marshal(buffer); 
    return socket->Send(buffer, record.Size(), 0); 
}