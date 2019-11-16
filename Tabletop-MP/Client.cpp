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

	std::cout << "Connected to " << ip << ":" << port << "!\n";

	enet_host_flush(client);
}

void Client::Socket::Update() {
	ENetEvent network_event;
	while (enet_host_service(client, &network_event, 0) > 0)
	{
		switch (network_event.type)
		{
		case ENET_EVENT_TYPE_CONNECT:
			break;
		case ENET_EVENT_TYPE_RECEIVE:
		{
			Serial::Packet packet(network_event.packet);
			HandlePacket(packet);
			enet_packet_destroy(network_event.packet);
		}
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

void Client::Socket::HandlePacket(Serial::Packet& packet) {
	unsigned char id = 0;
	packet >> id;
	switch ((MESSAGE_TYPE)id) {
	case MESSAGE_TYPE::UPDATE_TABLE:
		break;
	case MESSAGE_TYPE::UPDATE_ENTITY:
		UpdateEntity(packet);
		break;
	}
}

void Client::Socket::UpdateEntity(Serial::Packet& packet) {
	unsigned char table_id = 0, entity_id = 0, fun = 0;
	packet >> table_id >> entity_id >> fun;
	Tabletop::Entity &e = *owner->table.area_list[table_id].entity_list[entity_id];
	switch ((MESSAGE_ENTITY)fun) {
	case MESSAGE_ENTITY::ASSET_ID:
	{
		unsigned char asset_id = 0;
		packet >> asset_id;
		e.asset_id = asset_id;
	}
		break;
	case MESSAGE_ENTITY::FLIP:
		e.Flip();
		break;
	case MESSAGE_ENTITY::ROTATE:
	{
		float rotation = 0;
		packet >> rotation;
		e.rotation = rotation;
	}
		break;
	}
}

Client::Client() : socket(this), eman(this){

}

void Client::TableUpdate(sf::Event &e) {
	if (e.type == sf::Event::MouseButtonPressed) {
		auto et = GetEntityAt(0, sf::Vector2f(e.mouseButton.x, e.mouseButton.y - 20));
		if (et == nullptr) {
			std::cout << "Nothing was found under the cursor\n";
		}
		else {
			std::cout << "Found asset " << et->asset_id << " under the cursor.\n";
		}
	}
	else if (e.type == sf::Event::KeyPressed) {
		auto et = GetEntityAt(0, sf::Vector2f(mouse_table_position.x, mouse_table_position.y));
		if (et == nullptr)
			return;
		if (e.key.code == sf::Keyboard::Q)
			eman.Rotate(*et, -10);
		else if (e.key.code == sf::Keyboard::E)
			eman.Rotate(*et, 10);
		else if (e.key.code == sf::Keyboard::F)
			eman.Flip(*et);
	}
}

void Client::Update() {
	window.setView(current_view.view);
	mouse_table_position = window.mapPixelToCoords(sf::Mouse::getPosition(window));
	window.setView(sf::View(sf::FloatRect(0,0,1280, 720)));

	sf::Event window_event;
	while (window.pollEvent(window_event)) {
		if (window_event.type == sf::Event::Closed) {
			window.close();
		}
		else {
			if (sf::Mouse::getPosition(window).y > 20) {
				TableUpdate(window_event);
			}
			else {
				if (window_event.type == sf::Event::KeyPressed) {
					if (window_event.key.code == sf::Keyboard::Q) {
						current_view.view.rotate(-5);
						current_view.rotation -= 5;
					}
					else if (window_event.key.code == sf::Keyboard::E) {
						current_view.view.rotate(5);
						current_view.rotation += 5;
					}
					else if (window_event.key.code == sf::Keyboard::W) {
						float x = cos(((current_view.rotation + 90) / 180) * 3.14159265358979323846);
						float y = sin(((current_view.rotation + 90) / 180) * 3.14159265358979323846);
						current_view.view.move(sf::Vector2f(x * -10, y * -10));
					}	
					else if (window_event.key.code == sf::Keyboard::A) {
						float x = cos((current_view.rotation / 180) * 3.14159265358979323846);
						float y = sin((current_view.rotation / 180) * 3.14159265358979323846);
						current_view.view.move(sf::Vector2f(x * -10, y * -10));
					}
					else if (window_event.key.code == sf::Keyboard::S) {
						float x = cos(((current_view.rotation+90) / 180) * 3.14159265358979323846);
						float y = sin(((current_view.rotation+90) / 180) * 3.14159265358979323846);
						current_view.view.move(sf::Vector2f(x * 10, y*10));
					}
					else if (window_event.key.code == sf::Keyboard::D) {
						float x = cos((current_view.rotation / 180) * 3.14159265358979323846);
						float y = sin((current_view.rotation / 180) * 3.14159265358979323846);
						current_view.view.move(sf::Vector2f(x * 10, y * 10));
					}
						
				}
				else if (window_event.type == sf::Event::MouseWheelScrolled) {
					if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)){
						current_view.view.rotate(-window_event.mouseWheelScroll.delta * 3.6);
						current_view.rotation += -window_event.mouseWheelScroll.delta * 3.6;
					}
					else {
						current_view.view.zoom(1 + (window_event.mouseWheelScroll.delta * 0.05));
						current_view.zoom += window_event.mouseWheelScroll.delta * 0.05;
					}
				}
			}
		}
	}
}

void Client::Draw() {
	window.clear(sf::Color::White);
	table_renderer.clear(sf::Color(25, 25, 25, 255));
	for (auto& e : table.area_list[0].entity_list) {
		asset_manager.asset_list[e->asset_id]->Draw(table_renderer, *e);
	}
	table_renderer.setView(current_view.view);
	table_renderer.display();
	const sf::Texture table_texture = table_renderer.getTexture();
	sf::Sprite table_sprite(table_texture);
	table_sprite.setPosition(sf::Vector2f(0, 20));
	window.draw(table_sprite);
	window.display();
}

void Client::Run() {
	table_renderer.create(1280, 700);
	current_view.view.setSize(sf::Vector2f(1280, 700));
	current_view.view.setViewport(sf::FloatRect(0, 0, 1, 1));
	current_view.view.setCenter(sf::Vector2f(640, 350));
	window.create(sf::VideoMode(1280, 720), "Tabletop", 5U);
	asset_manager.AddSprite("Assets/back.png");
	asset_manager.AddSprite("Assets/2_of_clubs_th.jpg");
	asset_manager.AddSprite("Assets/board.jpg");

	socket.Connect();
	while(window.isOpen()){
		if (socket.connected) 
			socket.Update();
		Update();
		Draw();
	}
}

int main() {
	Client client;
	client.Run();
	return 0;
}

Tabletop::Entity* Client::GetEntityAt(unsigned char areaID, sf::Vector2f pos)
{
	for (int i = table.area_list[areaID].entity_list.size() - 1; i > -1; i--) {
		Tabletop::Entity& e = *table.area_list[areaID].entity_list[i];
		if (asset_manager.asset_list[e.asset_id]->CheckCollisionAtPoint(pos, e))
			return &e;
	}
	return nullptr;
}

void Client::EntityManipulator::Flip(Tabletop::Entity& e)
{
	e.Flip();
	Serial::Packet packet;
	packet << (unsigned char) MESSAGE_TYPE::UPDATE_ENTITY << (unsigned char)0 << e.id << (unsigned char)MESSAGE_ENTITY::FLIP;
	ENetPacket* p = packet.GetENetPacket();
	enet_peer_send(client->socket.server, 0, p);
	enet_host_flush(client->socket.client);
}

void Client::EntityManipulator::Rotate(Tabletop::Entity& e, float rotation)
{
	e.rotation += rotation;
	Serial::Packet packet;
	packet << (unsigned char)MESSAGE_TYPE::UPDATE_ENTITY << (unsigned char)0 << e.id << (unsigned char)MESSAGE_ENTITY::ROTATE << e.rotation;
	ENetPacket* p = packet.GetENetPacket();
	enet_peer_send(client->socket.server, 0, p);
	enet_host_flush(client->socket.client);
}
