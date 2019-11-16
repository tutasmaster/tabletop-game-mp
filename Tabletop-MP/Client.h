
#pragma once

#include "Tabletop-MP.h"

#define ENET_IMPLEMENTATION
#include "enet.h"
#include "SFML/Graphics.hpp"
#include "Serialization.hpp"
#include "Messages.h"

class Client {
public:
	class Socket {
	public:
		Socket(Client * client) : owner(client) {}

		void Connect(std::string ip, unsigned short port);
		void Update();
		void HandlePacket(Serial::Packet& packet);
		void UpdateEntity(Serial::Packet& packet);

		ENetHost* client = nullptr;
		ENetPeer* server = nullptr;
		Client* owner = nullptr;

		ENetAddress address = { 0 };

		bool connected = false;
	}socket;
	
	AssetManager asset_manager;
	Tabletop table;
	sf::RenderWindow window;
	void Run();


	void Update();
	void Draw();

	Client();
};