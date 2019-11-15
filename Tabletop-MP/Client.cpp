#include "Client.h"

void Client::Socket::Connect() {
	connected = true;
}

Client::Client() : socket(this){

}

void Client::Run() {

}

int main() {
	Client client;
	client.Run();
	return 0;
}