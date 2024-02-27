#include <string.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <netdb.h>
#include<iostream>
#include <netinet/tcp.h>
#include "ServerClientStub.h"

int ServerClientStub::Init(std::string ip, int port) {
	return socket.Init(ip, port);	
}

bool ServerClientStub::sendUpdate(CustomerRequests request) {
    char buffer[64];
	int size;
    request.Marshal(buffer);
	size = request.Size();
	return socket.Send(buffer, request.Size(), 0);
}
