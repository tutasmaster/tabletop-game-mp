#include "Client.h"

void Client::Socket::Connect(std::string ip = "127.0.0.1", unsigned short port = 7777) {
	connected = false;
	
	if (enet_initialize() != 0){
		std::cout << "FAILED TO INITIALIZE ENET!\n";
		return;
	}

	enet_address_set_host(&address, ip.c_str());
	address.port = port;

	client = enet_host_create(NULL, 1, 1, 0, 0);
	server = enet_host_connect(client, &address, 2, 0);

	ENetEvent e;
	if (enet_host_service(client, &e, 5000) > 0 && e.type == ENET_EVENT_TYPE_CONNECT)
		connected = true;
	else
		return;

	enet_host_flush(client);
}

Client::Client() : socket(this){

}

void Client::Run() {

}

int main() {
	Client client;
	client.Run();
	return 0;
}