#pragma once
#include <WinSock2.h>
#pragma  comment(lib,"ws2_32.lib")

class Client
{
public:
	Client();
	~Client();
private:
	void init();
	void sendMessage();
	SOCKET client;
};

