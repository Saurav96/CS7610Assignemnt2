#ifndef __CLIENT_STUB_H__
#define __CLIENT_STUB_H__

#include <string>
#include <iostream>
#include "ClientSocket.h"
#include "Messages.h"

class ClientStub {
private:
	ClientSocket socket;
public:
	ClientStub();
	int Init(std::string ip, int port);
	LaptopInfo OrderLaptop(CustomerRequests request);
	Customer_Record ReadRecord(CustomerRequests request);
};


#endif // end of #ifndef __CLIENT_STUB_H__
