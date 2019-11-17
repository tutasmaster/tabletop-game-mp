#include "Server.h"

void Server::Socket::Start() {
	connected = false;

	if (enet_initialize() != 0) { std::cout << "FAILED TO INITIALIZE ENET!\n"; return; }

	ENetAddress address = { 0 };
	address.host = ENET_HOST_ANY;
	address.port = 7777;

	server = enet_host_create(&address, 32, 2, 0, 0);

	if (server == NULL) { std::cout << "ENET FAILED TO BIND THE SOCKET!\n"; return; }

	std::cout << "Started a server...\n";
	connected = true;
}

void Server::Socket::Update() {
	ENetEvent network_event;
	enet_host_service(server, &network_event, 15);
	switch (network_event.type)
	{
	case ENET_EVENT_TYPE_CONNECT:
		OnConnect(network_event);
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

void Server::Socket::OnConnect(ENetEvent& e) {
	std::cout << "A client has connected!\n";
	User u;
	u.peer = e.peer;
	u.id = current_id++;
	connected_clients.push_back(u);
}

void Server::Socket::HandlePacket(Serial::Packet& packet) {
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

void Server::Socket::UpdateEntity(Serial::Packet& packet) {
	unsigned char table_id = 0, entity_id = 0, fun = 0;
	packet >> table_id >> entity_id >> fun;
	Tabletop::Entity * e = owner->table.area_list[table_id].FindEntity(entity_id);
	if (e == nullptr) {
		std::cout << "INVALID ENTITY UPDATED!\n";
		return;
	}
	switch ((MESSAGE_ENTITY)fun) {
	case MESSAGE_ENTITY::FLIP:
		std::cout << "AN ENTITY HAS BEEN FLIPPED!\n";
		owner->table.area_list[e->area_id].Flip(*e);
		BroadcastEntityStateUpdate(table_id, entity_id, (unsigned char)MESSAGE_ENTITY::FLIP);
		break;
	case MESSAGE_ENTITY::ROTATE:
		std::cout << "AN ENTITY HAS BEEN ROTATED!\n";
	{
		float rotation = 0;
		packet >> rotation;
		e->rotation = rotation;
		BroadcastEntityStateUpdate(table_id, entity_id, (unsigned char)MESSAGE_ENTITY::ROTATE);
	}
		break;
	case MESSAGE_ENTITY::POS:
		std::cout << "AN ENTITY HAS BEEN MOVED!\n";
	{
		float x = 0, y = 0;
		packet >> x >> y;
		e->x = x;
		e->y = y;
		BroadcastEntityStateUpdate(table_id, entity_id, (unsigned char)MESSAGE_ENTITY::POS);
	}
		break;
	case MESSAGE_ENTITY::POS_UNRELIABLE:
	{
		float x = 0, y = 0;
		packet >> x >> y;
		e->x = x;
		e->y = y;
		BroadcastEntityStateUpdate(table_id, entity_id, (unsigned char)MESSAGE_ENTITY::POS_UNRELIABLE);
	}
		break;
	}
}

void Server::Socket::BroadcastEntityStateUpdate(unsigned char table_id, unsigned char entity_id, unsigned char entity_function)
{
	Tabletop::Entity* e = owner->table.area_list[table_id].FindEntity(entity_id);
	if (e == nullptr) {
		std::cout << "INVALID ENTITY UPDATED!\n";
		return;
	}
	Serial::Packet p;
	p << (unsigned char) MESSAGE_TYPE::UPDATE_ENTITY << table_id << entity_id << entity_function;
	switch ((MESSAGE_ENTITY)entity_function) {
	case MESSAGE_ENTITY::ASSET_ID:
		p << e->asset_id;
		break;
	case MESSAGE_ENTITY::FLIP:
		p << e->asset_id;
		break;
	case MESSAGE_ENTITY::ROTATE:
		p << e->rotation;
		break;
	case MESSAGE_ENTITY::POS:
	case MESSAGE_ENTITY::POS_UNRELIABLE:
		p << e->x << e->y;
		break;
	}
	ENetPacket* packet = p.GetENetPacket(
		(MESSAGE_ENTITY)entity_function == MESSAGE_ENTITY::POS_UNRELIABLE ? 
		ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT : 
		ENET_PACKET_FLAG_RELIABLE
	);
	enet_host_broadcast(server, 0, packet);
	enet_host_flush(server);
}

Server::Server() : socket(this) {

}

void Server::Run() {
	socket.Start();
	while (socket.connected) {
		socket.Update();
	}
}

int main() {
	Server server;
	server.Run();
	return 0;
}