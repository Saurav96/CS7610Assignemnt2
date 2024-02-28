# Code Design

## Part 1:- 

We Introduced New Types

1. To update customer Data ./client 10.200.125.72 123456 4 3000 1
2. To get Current data from system ./client 10.200.125.71 12345 5 10 2
3. To Only print cusotmer data in ranges ./client 10.200.125.72 12345 1 10 3 

Introduction of ServerpeerInfo we keep track of all the peers in case a server is connecting with Peer.

class CustomerRequests {
private:
	int customer_id;
	int order_number;
	int request_type;
	int type_of_request;
	MapOp obj;
}

Introducted MapObj  the customerRequest Type.

## Part 2:- 

class CustomerRequests {
private:
	int customer_id;
	int order_number;
	int request_type;
	int type_of_request;
	MapOp obj;
	ServerReplication replication;
}

Added type_of_request as a paramater, the main reason to add if I am doing server-server/Replication communication, I can handle it with "1", if I am doing server-client communication, I can handle it with "0", this way my newStub dont have to create another request in Server, and we can utilize the same server stub.

Added ServerClientSocket and ServerClientStub to handle communication for "Replication".

## Running for One Server 
have to Give Unique Id and Number of peers :- 0
./server 123456 0 0
./client 10.200.125.71 123456 4 5000 1

## Runnin for Two Server

./server 123456 0 1 1 10.200.125.72 12345
./server 12345 1 2 0 10.200.125.71 123456


## Runnin for Three Servers

./server 123456 0 2 1 10.200.125.72 12345 2 10.200.125.73 123456
./server 12345 1 2 0 10.200.125.71 123456 2 10.200.125.73 123456 
./server 123456 2 2 0 10.200.125.71 123456 1 10.200.125.72 12345

## Running for Four Servers

./server 123456 0 3 1 10.200.125.72 12345 2 10.200.125.73 123456 3 10.200.125.74 123456
./server 12345 1 3 0 10.200.125.71 123456 2 10.200.125.73 123456 3 10.200.125.74 123456
./server 123456 2 3 0 10.200.125.71 123456 1 10.200.125.72 12345 3 10.200.125.74 123456
./server 123456 2 3 0 10.200.125.71 123456 1 10.200.125.72 12345 2 10.200.125.72 123456

## Runnin For 5 servers

./server 123456 0 4 1 10.200.125.72 12345 2 10.200.125.73 123456 3 10.200.125.74 123456 4 10.200.125.76 123456
./server 12345 1 4 0 10.200.125.71 123456 2 10.200.125.73 123456 3 10.200.125.74 123456 4 10.200.125.76 123456
./server 123456 2 4 0 10.200.125.71 123456 1 10.200.125.72 12345 3 10.200.125.74 123456 4 10.200.125.76 123456
./server 123456 2 4 0 10.200.125.71 123456 1 10.200.125.72 12345 2 10.200.125.74 123456 4 10.200.125.76 123456
./server 123456 2 4 0 10.200.125.71 123456 1 10.200.125.72 12345 2 10.200.125.73 123456 3 10.200.125.74 123456




