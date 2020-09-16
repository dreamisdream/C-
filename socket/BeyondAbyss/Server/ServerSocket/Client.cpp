#include "stdafx.h"
#include "Client.h"

#define IP "192.168.0.158"
#define PORT 8888

Client::Client()
{
	init();
}


Client::~Client()
{
}

void Client::init()
{
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	client = socket(AF_INET, SOCK_STREAM, 0);
	
	SOCKADDR_IN addr_client;
	addr_client.sin_family = AF_INET;
	addr_client.sin_port = htons(PORT);
	addr_client.sin_addr.S_un.S_addr = inet_addr(IP);
	connect(client, (sockaddr*)&addr_client, sizeof(addr_client));

}

void Client::sendData()
{
}

void Client::recvData()
{
}
