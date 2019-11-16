
#pragma once

#include "Tabletop-MP.h"

#define ENET_IMPLEMENTATION
#include "enet.h"

class Server {
public:
	class Socket {
	public:
		class User {
			ENetPeer* peer = nullptr;
			Tabletop::Player* player = nullptr;
		};
		Socket(Server* server) : owner(server) {}

		void Start();
		void Update();

		std::vector<User> connected_clients;

		ENetHost* server = nullptr;
		Server* owner = nullptr;

		bool connected = false;
	}socket;
	Tabletop table;
	void Run();

	Server();
};