#pragma once

#include <iostream>
#include <WinSock2.h>

#pragma comment(lib,"ws2_32.lib")

class Client
{
public:
	Client();
	~Client();

	void init();
	void sendData();
	void recvData();
	SOCKET client;
};

