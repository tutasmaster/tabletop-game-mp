
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
		void SetRotation(Tabletop::Entity& e, float rotation);
		void Move(Tabletop::Entity& e, sf::Vector2f position);
		void MoveUnreliable(Tabletop::Entity& e, sf::Vector2f position);
		void Remove(Tabletop::Entity& e);
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

	unsigned int WIDTH = 400, HEIGHT = 200, FPS_CAP = 60;

	struct Widgets {
		tgui::Gui gui;
		tgui::Panel::Ptr panel;
		tgui::Button::Ptr flip_button;
		tgui::Button::Ptr reset_rotation_button;
		tgui::Button::Ptr erase_button; 
		Widgets() {
			flip_button = tgui::Button::create("FLIP");
			reset_rotation_button = tgui::Button::create("RESET ROTATION");
			erase_button = tgui::Button::create("ERASE");
			panel = tgui::Panel::create();
			panel->add(flip_button);
			panel->add(reset_rotation_button);
			panel->add(erase_button);
			reset_rotation_button->setPosition(0, flip_button->getSize().y);
			erase_button->setPosition(0, flip_button->getSize().y * 2);
			panel->setSize(reset_rotation_button->getSize().x, reset_rotation_button->getSize().y*3);
			panel->setEnabled(false);
			panel->setVisible(false);
			gui.add(panel, "CARD");
			panel->getRenderer()->setBackgroundColor(sf::Color(0, 0, 0, 0));
		}

		void FlipEntity(unsigned char area, unsigned char id) {
			client->eman.Flip(*client->table.area_list[area]->FindEntity(id));
			panel->setEnabled(false);
			panel->setVisible(false);
		}

		void ResetRotation(unsigned char area, unsigned char id) {
			client->eman.SetRotation(*client->table.area_list[area]->FindEntity(id),0);
			panel->setEnabled(false);
			panel->setVisible(false);
		}

		void EraseEntity(unsigned char area, unsigned char id) {
			client->eman.Remove(*client->table.area_list[area]->FindEntity(id));
			panel->setEnabled(false);
			panel->setVisible(false);
		}

		void OpenEntity(sf::Vector2f pos, unsigned char area, unsigned char id) {
			panel->setPosition(pos.x, pos.y);
			panel->setVisible(true);
			panel->setEnabled(true);
			flip_button->disconnectAll();
			flip_button->connect("pressed",&Client::Widgets::FlipEntity,this,area,id);
			reset_rotation_button->disconnectAll();
			reset_rotation_button->connect("pressed", &Client::Widgets::ResetRotation, this, area, id);
			erase_button->disconnectAll();
			erase_button->connect("pressed", &Client::Widgets::EraseEntity, this, area, id);
		}

		Client* client;
	}widgets;

	Client();
};