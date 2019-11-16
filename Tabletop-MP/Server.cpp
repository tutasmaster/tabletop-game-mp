#include "Server.h"

void Server::Socket::Start() {
	connected = false;

	if (enet_initialize() != 0) { std::cout << "FAILED TO INITIALIZE ENET!\n"; return; }

	ENetAddress address = { 0 };
	address.host = ENET_HOST_ANY;
	address.port = 7777;

	server = enet_host_create(&address, 32, 2, 0, 0);

	if (server == NULL) { std::cout << "ENET FAILED TO BIND THE SOCKET!\n"; return; }

	std::cout << "Started a server...\n";
	connected = true;
}

void Server::Socket::Update() {
	ENetEvent network_event;
	while (true)
	{
		enet_host_service(server, &network_event, 15);
		switch (network_event.type)
		{
		case ENET_EVENT_TYPE_CONNECT:
			break;
		case ENET_EVENT_TYPE_RECEIVE:
			enet_packet_destroy(network_event.packet);
			break;
		case ENET_EVENT_TYPE_DISCONNECT:
			break;
		case ENET_EVENT_TYPE_DISCONNECT_TIMEOUT:
			break;
		default:
			break;
		}
	}
}

Server::Server() : socket(this) {

}

void Server::Run() {
	socket.Start();
	while (socket.connected) {
		socket.Update();
	}
}

int main() {
	Server server;
	server.Run();
	return 0;
}