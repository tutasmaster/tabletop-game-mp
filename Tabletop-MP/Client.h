
#pragma once

#include "Tabletop-MP.h"

#include "enet.h"
#include "SFML/Graphics.hpp"

class Client {
public:
	class Socket {
	public:
		Socket(Client * client) : owner(client) {}

		void Connect(std::string ip = "127.0.0.1", unsigned short port = 7777);
		void Update();

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

	Client();
};