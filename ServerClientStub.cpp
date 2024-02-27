#include <string.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <netdb.h>
#include<iostream>
#include <netinet/tcp.h>
#include "ServerClientStub.h"

int ServerClientStub::Init(std::string ip, int port) {
    try {
	    return socket.Init(ip, port);
    }
    catch (const std::runtime_error& e){
        
        return 0;
    }	
}

bool ServerClientStub::sendUpdate(CustomerRequests request) {
    char buffer[64];
    request.Marshal(buffer);
	return socket.Send(buffer, request.Size(), 0);
}
