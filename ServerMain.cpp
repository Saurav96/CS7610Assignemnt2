#include <chrono>
#include <iostream>
#include "ServerSocket.h"
#include "ServerThread.h"
#include "ServerPeerInfo.h"

int main(int argc, char *argv[]) {
	int port;
	int engineer_cnt = 0;
	int peer = 0;

	ServerSocket socket;
	LaptopFactory factory;
	std::unique_ptr<ServerSocket> new_socket;
	std::vector<std::thread> thread_vector;
	
	 if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " [port #] [unique ID] [# peers] (repeat [ID] [IP] [port #])\n";
        return 1;
    }

	port = atoi(argv[1]);
	unique_id = atoi(argv[2]);
	peer = atoi(argv[3]);

	PeerInfo obj1(unique_id, "127.0.0.1", port);
	ifaAddresses.emplace_back(obj1);
	for(int i = 0;i< peer;++i){
		int peerIndex = 4 + i*3;
		if(peerIndex+2 < argc+1){
			int peerID = std::stoi(argv[peerIndex]);
            std::string peerIP = argv[peerIndex + 1];
            int peerPort = std::stoi(argv[peerIndex + 2]);
			PeerInfo obj(peerID, peerIP, peerPort);
            ifaAddresses.emplace_back(obj);
        } else {
            std::cerr << "Error: Insufficient information for peer " << i + 1 << "\n";
            return 1;

		}
	}

	std::thread expert_thread(&LaptopFactory::AdminThread, 
				&factory, engineer_cnt++);
	thread_vector.push_back(std::move(expert_thread));

	if (!socket.Init(port)) {
		std::cout << "Socket initialization failed" << std::endl;
		return 0;
	}

	while ((new_socket = socket.Accept())) {
		std::thread engineer_thread(&LaptopFactory::EngineerThread, 
				&factory, std::move(new_socket), 
				engineer_cnt++);
		thread_vector.push_back(std::move(engineer_thread));
	}
	return 0;
}
