#ifndef SERVERCLIENTSTUB_H
#define SERVERCLIENTSTUB_H

#include "ClientSocket.h" 
#include "Socket.h"
#include "Messages.h"
#include "ServerClientSocket.h"
#include <string>

class ServerClientStub :  public Socket{
private:
    ServerClientSocket socket; 
public:
    ServerClientStub(){};
    int Init(std::string ip, int port);
    bool sendUpdate(CustomerRequests request);
};

#endif // SERVERCLIENTSTUB_H
