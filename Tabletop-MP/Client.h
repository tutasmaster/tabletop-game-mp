
#pragma once

#include "Tabletop-MP.h"

#include "enet.h"
#include "SFML/Graphics.hpp"

class Client {
public:
	class Socket {
	public:
		Socket(Client * client) : owner(client) {}

		void Connect();

		ENetHost* client = nullptr;
		ENetPeer* server = nullptr;
		Client* owner = nullptr;

		bool connected = false;
	}socket;

	AssetManager asset_manager;
	Tabletop table;
	sf::RenderWindow window;
	void Run();

	Client();
};