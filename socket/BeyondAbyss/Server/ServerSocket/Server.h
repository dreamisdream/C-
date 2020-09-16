#pragma once

#include "DataManager.h"

#include <winsock2.h>
#include <map>

#pragma comment(lib,"ws2_32.lib")

class Server
{
public:
	Server();
	~Server();
	void run();
private:
	void init();
	void recvData(SOCKET c);
	bool dealError(int num,char *data =nullptr);
	void heartCheck(); // ĞÄÌø¼ì²âÏß³Ì
	SOCKET m_server;
	SOCKET m_client;
	std::map<SOCKET, int> m_heart_check;
	bool flag;
	std::map<SOCKET, DataManager*> dataManagerSocket;

};

