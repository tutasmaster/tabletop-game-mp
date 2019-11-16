#include "Server.h"

void Server::Socket::Start() {
	running = true;
}

Server::Server() : socket(this) {

}

void Server::Run() {

}

int main() {
	Server server;
	server.Run();
	return 0;
}