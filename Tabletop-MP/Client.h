
#pragma once

#include "Tabletop-MP.h"

#define ENET_IMPLEMENTATION
#include "enet.h"
#include "SFML/Graphics.hpp"
#include "TGUI/TGUI.hpp"
#include "Serialization.hpp"
#include "Messages.h"
#include "Interpolation.h"

class Client {
public:
	class Socket {
	public:
		Socket(Client * client) : owner(client) {}

		void Connect(std::string ip, unsigned short port);
		void Update(float time);
		void HandlePacket(Serial::Packet& packet);
		void UpdateEntity(Serial::Packet& packet);
		void UpdateTable (Serial::Packet& packet);

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

	void Update(float time);
	void Draw(float time);

	sf::Clock timer;

	unsigned int WIDTH = 400, HEIGHT = 200;

	struct Widgets {
		tgui::Gui gui;
		tgui::Panel::Ptr panel;
		tgui::Button::Ptr flip_button;
		Widgets() {
			flip_button = tgui::Button::create("FLIP");
			panel = tgui::Panel::create();
			panel->add(flip_button);
			panel->setSize(flip_button->getSize());
			panel->setEnabled(false);
			panel->setVisible(false);
			gui.add(panel, "CARD");
		}

		void OpenEntity(sf::Vector2f pos, unsigned char area, unsigned char id) {
			panel->setPosition(pos.x, pos.y);
			panel->setVisible(true);
			panel->setEnabled(true);
		}
	}widgets;

	Client();
};