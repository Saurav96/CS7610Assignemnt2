#ifndef __SERVER_STUB_H__
#define __SERVER_STUB_H__

#include <memory>
#include <iostream>
#include "ServerSocket.h"
#include "Messages.h"

class ServerStub {
private:
	std::unique_ptr<ServerSocket> socket;
public:
	ServerStub();
	void Init(std::unique_ptr<ServerSocket> socket);
	CustomerRequests ReceiveRequest();
	int ShipLaptop(LaptopInfo info);
	int ReturnRecord(Customer_Record record);
};

#endif // end of #ifndef __SERVER_STUB_H__
