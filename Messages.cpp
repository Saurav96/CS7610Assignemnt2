#include <cstring>
#include <iostream>

#include <arpa/inet.h>
#include "Messages.h"

CustomerRequests::CustomerRequests() {
	customer_id = -1;
	order_number = -1;
	request_type = -1;
	type_of_request = -1;
	MapOp obj;
	ServerReplication replication;
}

void CustomerRequests::SetOrder(int id, int number, int type, int request, MapOp new_obj, ServerReplication ServerReplication) {
	customer_id = id;
	order_number = number;
	request_type = type;
	type_of_request = request;
	obj = new_obj;
	replication = ServerReplication;
}

void CustomerRequests::setMapObj(MapOp obj1) {
	this->obj = obj1;
}


int CustomerRequests::GetCustomerId() { return customer_id; }
int CustomerRequests::GetOrderNumber() { return order_number; }
int CustomerRequests::GetRequestType() { return request_type; }
int CustomerRequests::GetTypeOfRequest() { return type_of_request;}
MapOp CustomerRequests::GetMapObj() { return obj;}
ServerReplication CustomerRequests::GetReplication() {return replication;}


int CustomerRequests::Size() {
	return sizeof(customer_id) + sizeof(order_number) + sizeof(request_type) + sizeof(type_of_request) + sizeof(obj) 
	+ sizeof(replication);
}

void CustomerRequests::Marshal(char *buffer) {
	int net_customer_id = htonl(customer_id);
	int net_order_number = htonl(order_number);
	int net_request_type = htonl(request_type);
	int net_type_of_request = htonl(type_of_request);
	int offset = 0;
	//std::cout<<"Marshall "<<customer_id<<" "<<order_number<<" "<<request_type<<" "<<type_of_request<<std::endl;
	memcpy(buffer + offset, &net_customer_id, sizeof(net_customer_id));
	offset += sizeof(net_customer_id);
	memcpy(buffer + offset, &net_order_number, sizeof(net_order_number));
	offset += sizeof(net_order_number);
	memcpy(buffer + offset, &net_request_type, sizeof(net_request_type));
	offset += sizeof(net_request_type);
	memcpy(buffer + offset, &net_type_of_request, sizeof(net_type_of_request));
	offset += sizeof(net_type_of_request);
	obj.Marshal(buffer, offset);
	replication.Marshal(buffer, offset);
}

void CustomerRequests::Unmarshal(char *buffer) {
	int net_customer_id = -1;
	int net_order_number = -1;
	int net_request_type = -1;
	int net_type_of_request = -1;
	int offset = 0;
	memcpy(&net_customer_id, buffer + offset, sizeof(net_customer_id));
	offset += sizeof(net_customer_id);
	memcpy(&net_order_number, buffer + offset, sizeof(net_order_number));
	offset += sizeof(net_order_number);
	memcpy(&net_request_type, buffer + offset, sizeof(net_request_type));
	offset += sizeof(net_request_type);
	memcpy(&net_type_of_request, buffer + offset, sizeof(net_type_of_request));
	offset += sizeof(net_type_of_request);
	
	customer_id = ntohl(net_customer_id);
	order_number = ntohl(net_order_number);
	request_type = ntohl(net_request_type);
	type_of_request = ntohl(net_type_of_request);
	//std::cout<<"UnMarshall "<<customer_id<<" "<<order_number<<" "<<request_type<<" "<<type_of_request<<std::endl;
	obj.Unmarshal(buffer, offset);
	replication.Unmarshal(buffer, offset);

}

bool CustomerRequests::IsValid() {
	return (customer_id != -1);
}

MapOp::MapOp() : opcode(-1), arg1(-1), arg2(-1) {}

void MapOp::SetOpcode(int opcode) {
    this->opcode = opcode;
}

void MapOp::SetArg1(int arg1) {
    this->arg1 = arg1;
}

void MapOp::SetArg2(int arg2) {
    this->arg2 = arg2;
}

int MapOp::GetOpcode() const {
    return this->opcode;
}

int MapOp::GetArg1() const {
    return this->arg1;
}

int MapOp::GetArg2() const {
    return this->arg2;
}

void MapOp::Marshal(char* buffer, int &offset)  {
    int net_opcode = htonl(opcode);
    int net_arg1 = htonl(arg1);
    int net_arg2 = htonl(arg2);

	//std::cout<<"marshall "<<opcode<<" "<<arg1<<" "<<arg2<<std::endl;
    memcpy(buffer + offset, &net_opcode, sizeof(net_opcode));
    offset += sizeof(net_opcode);
    memcpy(buffer + offset, &net_arg1, sizeof(net_arg1));
    offset += sizeof(net_arg1);
    memcpy(buffer + offset, &net_arg2, sizeof(net_arg2));
    offset += sizeof(net_arg2);


}

void MapOp::Unmarshal(char* buffer, int &offset) {
    int net_opcode = -1;
    int net_arg1 = -1;
    int net_arg2= -1;
	ServerReplication replication;
    memcpy(&net_opcode, buffer + offset, sizeof(net_opcode));
    offset += sizeof(net_opcode);
    memcpy(&net_arg1, buffer + offset, sizeof(net_arg1));
    offset += sizeof(net_arg1);
	
    memcpy(&net_arg2, buffer + offset, sizeof(net_arg2));
    offset += sizeof(net_arg2);
	
    opcode = ntohl(net_opcode);
    arg1 = ntohl(net_arg1);
    arg2 = ntohl(net_arg2);

	//std::cout<<"UnMarshall "<< opcode<<" "<<arg1<<" "<<arg2<<std::endl;
}

void MapOp::print() {
	 std::cout << "net_opcode: " << opcode << std::endl;
	 std::cout<< "net_arg1 " << arg1<<std::endl;
	 std::cout<< "net_arg2 " <<arg2<<std::endl;
}

ServerReplication::ServerReplication() : last_index(-1), commited_index(-1), primary_id(-1), factory_id(-1) {}

ServerReplication::ServerReplication(int last_index, int commited_index, int primary_id, int factory_id) 
    : last_index(last_index), commited_index(commited_index), primary_id(primary_id), factory_id(factory_id) {}

void ServerReplication::Marshal(char* buffer, int &offset) {
	
    int net_last_index = htonl(last_index);
	int net_commited_index = htonl(commited_index);
	int net_primary_id = htonl(primary_id);
	int net_factory_id = htonl(factory_id);
	//std::cout<<"marshall"<<std::endl;
	//std::cout<<last_index<<" "<<commited_index<<" "<<primary_id<<" "<<factory_id<<std::endl;
    memcpy(buffer + offset, &net_last_index, sizeof(net_last_index));
    offset += sizeof(net_last_index);
	
    memcpy(buffer + offset, &net_commited_index, sizeof(net_commited_index));
    offset += sizeof(net_commited_index); 

    memcpy(buffer + offset, &net_primary_id, sizeof(net_primary_id));
    offset += sizeof(net_primary_id);

    memcpy(buffer + offset, &net_factory_id, sizeof(net_factory_id));
	offset += sizeof(net_factory_id);
}


 void ServerReplication::Unmarshal(char* buffer, int &offset) {

    int net_last_index = -1;
	int net_commited_index = -1;
	int net_primary_id = -1;
	int net_factory_id = -1;

    memcpy(&net_last_index, buffer + offset, sizeof(net_last_index));
    offset += sizeof(net_last_index);
	
    memcpy(&net_commited_index, buffer + offset, sizeof(net_commited_index));  
    offset += sizeof(net_commited_index);
	
    memcpy(&net_primary_id, buffer + offset, sizeof(net_primary_id));
    offset += sizeof(net_primary_id);
	
    memcpy(&net_factory_id, buffer + offset, sizeof(net_factory_id));
    offset += sizeof(net_factory_id);
	

	last_index = ntohl(net_last_index);
	commited_index = ntohl(net_commited_index);
	primary_id = ntohl(net_primary_id);
	factory_id = ntohl(net_factory_id);
	//std::cout<<std::endl;
	//std::cout<<last_index<<" "<<commited_index<<" "<<primary_id<<" "<<factory_id<<std::endl;

}

void ServerReplication::print(){
	    std::cout << "net_last_index: " << last_index << std::endl;
        std::cout << "net_committed_index: " << commited_index << std::endl;
        std::cout << "net_primary_id: " << primary_id << std::endl;
        std::cout << "net_factory_id: " << factory_id << std::endl;

}


void ServerReplication::setLastIndex(int lastIndex) { this->last_index = lastIndex; }
void ServerReplication::setCommitedIndex(int commitedIndex) { this->commited_index = commitedIndex; }
void ServerReplication::setPrimaryId(int primaryId) { this->primary_id = primaryId; }
void ServerReplication::setFactoryId(int factoryId) { this->factory_id = factoryId; }

int ServerReplication::getLastIndex() const { return this->last_index; }
int ServerReplication::getCommitedIndex() const { return this->commited_index; }
int ServerReplication::getPrimaryId() const { return this->primary_id; }
int ServerReplication::getFactoryId() const { return this->factory_id; }

void CustomerRequests::Print() {
	std::cout << "id " << customer_id << " ";
	std::cout << "num " << order_number << " ";
	std::cout << "type " << request_type << std::endl;
	std::cout << "type_of_request" << type_of_request <<std::endl;
}

LaptopInfo::LaptopInfo() {
	customer_id = -1;
	order_number = -1;
	request_type  = -1;
	engineer_id = -1;
	admin_id = -1;
}

void LaptopInfo::SetInfo(int id, int number, int type, int engid, int adminid) {
	customer_id = id;
	order_number = number;
	request_type = type;
	engineer_id = engid;
	admin_id = adminid;
}

void LaptopInfo::CopyOrder(CustomerRequests order) {
	customer_id = order.GetCustomerId();
	order_number = order.GetOrderNumber();
	request_type = order.GetRequestType();
}
void LaptopInfo::SetEngineerId(int id) { engineer_id = id; }
void LaptopInfo::SetAdminId(int id) { admin_id = id; }
int LaptopInfo::GetCustomerId() { return customer_id; }
int LaptopInfo::GetOrderNumber() { return order_number; }
int LaptopInfo::GetRequestType() { return request_type; }
int LaptopInfo::GetEngineerId() { return engineer_id; }
int LaptopInfo::GetAdminId() {return admin_id;}
int LaptopInfo::Size() {
	return sizeof(customer_id) + sizeof(order_number) + sizeof(request_type)
		+ sizeof(engineer_id) + sizeof(admin_id);
}

void LaptopInfo::Marshal(char *buffer) {
	int net_customer_id = htonl(customer_id);
	int net_order_number = htonl(order_number);
	int net_request_type = htonl(request_type);
	int net_engineer_id = htonl(engineer_id);
	int net_admin_id = htonl(admin_id);
	int offset = 0;

	memcpy(buffer + offset, &net_customer_id, sizeof(net_customer_id));
	offset += sizeof(net_customer_id);
	memcpy(buffer + offset, &net_order_number, sizeof(net_order_number));
	offset += sizeof(net_order_number);
	memcpy(buffer + offset, &net_request_type, sizeof(net_request_type));
	offset += sizeof(net_request_type);
	memcpy(buffer + offset, &net_engineer_id, sizeof(net_engineer_id));
	offset += sizeof(net_engineer_id);
	memcpy(buffer + offset, &net_admin_id, sizeof(net_admin_id));
	offset += sizeof(net_admin_id);

}

void LaptopInfo::Unmarshal(char *buffer) {
	int net_customer_id;
	int net_order_number;
	int net_laptop_type;
	int net_engineer_id;
	int net_admin_id;
	int offset = 0;

	memcpy(&net_customer_id, buffer + offset, sizeof(net_customer_id));
	offset += sizeof(net_customer_id);
	memcpy(&net_order_number, buffer + offset, sizeof(net_order_number));
	offset += sizeof(net_order_number);
	memcpy(&net_laptop_type, buffer + offset, sizeof(net_laptop_type));
	offset += sizeof(net_laptop_type);
	memcpy(&net_engineer_id, buffer + offset, sizeof(net_engineer_id));
	offset += sizeof(net_engineer_id);
	memcpy(&net_admin_id, buffer+ offset, sizeof(net_admin_id));
	offset += sizeof(net_admin_id);

	customer_id = ntohl(net_customer_id);
	order_number = ntohl(net_order_number);
	request_type = ntohl(net_laptop_type);
	engineer_id = ntohl(net_engineer_id);
	admin_id = ntohl(net_admin_id);
}

bool LaptopInfo::IsValid() {
	return (customer_id != -1);
}

void LaptopInfo::Print() {
	std::cout << "id " << customer_id << " ";
	std::cout << "num " << order_number << " ";
	std::cout << "type " << request_type << " ";
	std::cout << "engid " << engineer_id << " ";
	std::cout << "adminid" << admin_id << " "<<std::endl;
}


Customer_Record::Customer_Record() {
	customer_id = -1;
	last_order = -1;
}

void Customer_Record::SetInfo(int id, int order){
	customer_id = id;
	last_order = order;
}

void Customer_Record::SetLastOrder(int id){

}

void Customer_Record::SetCustomerId(int id){

}

int Customer_Record::GetCustomerId(){
	return customer_id;
}

int Customer_Record::GetLastOrder() {
	return last_order;
}

int Customer_Record::Size() {
	return sizeof(customer_id) + sizeof(last_order);
}

void Customer_Record::Marshal(char *buffer) {
	int net_customer_id = htonl(customer_id);
	int net_last_order = htonl(last_order);
	int offset = 0;

	memcpy(buffer + offset, &net_customer_id, sizeof(net_customer_id));
	offset += sizeof(net_customer_id);
	memcpy(buffer + offset, &net_last_order, sizeof(net_last_order));
	offset += sizeof(net_last_order);
}

void Customer_Record::Unmarshal(char *buffer) {
	int net_customer_id;
	int net_last_order;
	int offset = 0;

	memcpy(&net_customer_id, buffer + offset, sizeof(net_customer_id));
	offset += sizeof(net_customer_id);
	memcpy(&net_last_order, buffer + offset, sizeof(net_last_order));
	offset += sizeof(net_last_order);

	customer_id = ntohl(net_customer_id);
	last_order = ntohl(net_last_order);
}

bool Customer_Record::IsValid(){
	return (customer_id != -1);
}


