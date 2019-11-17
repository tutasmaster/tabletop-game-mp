
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
		void Update(float time);
		void HandlePacket(Serial::Packet& packet);
		void UpdateEntity(Serial::Packet& packet);

		ENetHost* client = nullptr;
		ENetPeer* server = nullptr;
		Client* owner = nullptr;

		ENetAddress address = { 0 };

		float update_rate = 0.1;
		float accumulator = 0;
		ENetPacket * latest_packet = nullptr;

		bool connected = false;
	}socket;
	
	AssetManager asset_manager;
	Tabletop table;
	sf::RenderWindow window;
	sf::RenderTexture table_renderer;

	struct View{
		sf::View view;
		float rotation = 0;
		float zoom = 1;
	}current_view;

	struct EntityManipulator {
		EntityManipulator(Client * c) : client(c) {}
		void Flip(Tabletop::Entity& e);
		void Rotate(Tabletop::Entity& e, float rotation);
		void Move(Tabletop::Entity& e, sf::Vector2f position);
		void MoveUnreliable(Tabletop::Entity& e, sf::Vector2f position);
		Client* client;
	}eman;

	sf::Vector2f mouse_table_position;

	Tabletop::Entity* GetEntityAt(unsigned char areaID, sf::Vector2f pos);

	Tabletop::Entity* current_entity = nullptr;

	void Run();

	void TableUpdate(sf::Event &e);

	void Update();
	void Draw();

	sf::Clock timer;

	Client();
};