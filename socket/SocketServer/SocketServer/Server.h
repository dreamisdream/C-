#pragma once

#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
#include <map>

class Server
{
public:
	Server();
	~Server();
	void run();
	
private:
	void init();
	void checkClientHeart();
	void readMessage(SOCKET c);
	SOCKET m_server;
	std::map<SOCKET, int> m_mapClient;
	int flag;
};

