#include "stdafx.h"
#include "Server.h"
#include "common.h"

#include <thread>
#include <iostream>

Server::Server()
{
	init();
	std::thread t_checkHeart(&Server::checkClientHeart, this);
	t_checkHeart.detach();
}


Server::~Server()
{
	WSACleanup(); //清理动态库
	for (auto con : m_mapClient) {
		closesocket(con.first);
	}
}

void Server::run()
{
	if (flag)
		std::cout << "创建socket失败" << std::endl;
	while (true) {
		SOCKADDR_IN m_client;
		int len = sizeof(SOCKADDR_IN);
		std::cout << "等待客户端连接..." << std::endl;
		SOCKET client = accept(m_server, (sockaddr*)&m_client, &len);
		if(client <= 0)
			continue;
		std::cout <<"客户端:"<<client<< "连接成功" << std::endl;
		m_mapClient.insert(std::pair<SOCKET, int>(client, heartCount));
		std::thread t_read(&Server::readMessage, this, client);
		t_read.detach();
	}
}

void Server::init()
{
	WSADATA wsa;
	int ret =WSAStartup(0x0202, &wsa);
	m_server = socket(AF_INET, SOCK_STREAM, 0);
	std::cout << "m_server:" << m_server << std::endl;
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(c_port);
	addr.sin_addr.S_un.S_addr = inet_addr(c_ip);

	int bindRet = ::bind(m_server, (sockaddr*)&addr, sizeof(addr));
	if (bindRet == SOCKET_ERROR)
		flag = 1;
	listen(m_server, 5);

	//std::thread t_accpet(&Server::run, this);
	//t_accpet.detach();
}

void Server::checkClientHeart()
{
	std::cout << "*****************checkClientHeart****************" << std::endl;
	while (true) {
		for (auto it = m_mapClient.begin(); it != m_mapClient.end(); ) {
			it->second--;
			if (it->second == 0) {
				closesocket(it->first);
				std::cout << "客户端:" << it->first << " 关闭了" << std::endl;
				m_mapClient.erase(it++);
			}
			else
				it++;
		}
		Sleep(2000);
	}
}

void Server::readMessage(SOCKET c)
{
	while (true) {
		char buffer[1024] = { 0 };
		int ret = recv(c, buffer, 1024, 0);
		if (ret > 0) {
			m_mapClient[c] = heartCount;
			std::cout << "readMessage:" << buffer << "from:" << c << std::endl;
		}
	}
}
