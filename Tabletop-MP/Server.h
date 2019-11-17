
#pragma once

#include "Tabletop-MP.h"

#define ENET_IMPLEMENTATION
#include "enet.h"
#include "Serialization.hpp"
#include "Messages.h"

class Server {
public:
	class Socket {
	public:
		class User {
		public:
			ENetPeer* peer = nullptr;
			Tabletop::Player* player = nullptr;
			unsigned char id = 0;
		};
		Socket(Server* server) : owner(server) {}

		void Start();
		void Update();
		void OnConnect(ENetEvent& e);
		void HandlePacket(Serial::Packet& packet);
		void UpdateEntity(Serial::Packet& packet);

		void BroadcastEntityStateUpdate(
			unsigned char table_id, 
			unsigned char entity_id, 
			unsigned char entity_function
		);

		std::vector<User> connected_clients;

		ENetHost* server = nullptr;
		Server* owner = nullptr;

		unsigned char current_id = 0;

		bool connected = false;
	}socket;

	Tabletop table;
	void Run();

	Server();
};