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
	case MESSAGE_ENTITY::FLIP:
		e.Flip();
		break;
	}
}

Client::Client() : socket(this){

}

void Client::Update() {
	sf::Event window_event;
	while (window.pollEvent(window_event)) {
		if (window_event.type == sf::Event::Closed)
			window.close();
	}
}

void Client::Draw() {
	window.clear();
	sf::RectangleShape background;
	background.setFillColor(sf::Color(25, 25, 25, 255));
	background.setSize(sf::Vector2f(table.area_list[0].width, table.area_list[0].height));
	window.draw(background);
	for (auto& e : table.area_list[0].entity_list) {
		asset_manager.asset_list[e->asset_id]->Draw(window, *e);
	}
	window.display();
}

void Client::Run() {
	window.create(sf::VideoMode(1280, 720), "Tabletop", 5U);
	asset_manager.AddSprite("Assets/back.png");
	asset_manager.AddSprite("Assets/2_of_clubs_th.jpg");

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