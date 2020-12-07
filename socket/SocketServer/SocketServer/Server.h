#pragma once

#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
#include <map>
#include <string>

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
	bool ShareMessage(SOCKET c,std::string &message);
	std::string getLocalTime();
	SOCKET m_server;
	std::map<SOCKET, int> m_mapClient;
	int flag;
};

