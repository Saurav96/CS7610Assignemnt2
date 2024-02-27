#ifndef __SERVERPEERINFO_H__
#define __SERVERPEERINFO_H__

#include<string>
#include<vector>

class PeerInfo {
    int id = 0;
    std::string ip;
    int port;
    
    public:
    std::vector<PeerInfo> peers;
    int getPeerId(){
        return id;
    }

    std::string getIP(){
        return ip;
    }

    int getPort(){
        return port;
    }
    PeerInfo(int id, std::string ip, int port) : id(id), ip(ip), port(port) {}

};
extern int unique_id ;
extern std::vector<PeerInfo> ifaAddresses;
#endif