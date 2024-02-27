#include <array>
#include <iostream> 
#include <iomanip> 
#include <thread> 
#include <vector> 

#include "ClientSocket.h"
#include "ClientThread.h"
#include "ClientTimer.h"

int main(int argc, char *argv[]) {
	std::string ip;
	int port;
	int customer_id;
	int num_orders;
	int laptop_type;
	ClientTimer timer;

	std::vector<std::shared_ptr<ClientThreadClass>> client_vector;
	std::vector<std::thread> thread_vector;
	
	if (argc < 6) {
		std::cout << "not enough arguments" << std::endl;
		std::cout << argv[0] << "[ip] [port #] [# customers] ";
		std::cout << "[# orders] [laptop type 0 ]" << std::endl;
		return 0;
	}

	ip = argv[1];
	port = atoi(argv[2]);
	customer_id = atoi(argv[3]);
	num_orders = atoi(argv[4]);
	laptop_type = atoi(argv[5]);

	timer.Start();
		auto client_cls = std::shared_ptr<ClientThreadClass>(new ClientThreadClass());
		std::thread client_thread(&ClientThreadClass::ThreadBody, client_cls,
				ip, port, customer_id, num_orders, laptop_type);

		client_vector.push_back(std::move(client_cls));
		thread_vector.push_back(std::move(client_thread));
	for (auto& th : thread_vector) {
		th.join();
	}
	timer.End();


	return 1;
}
