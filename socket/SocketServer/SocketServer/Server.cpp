#include "stdafx.h"
#include "Server.h"
#include "common.h"

#include <thread>
#include <iostream>

using namespace std;

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
	if (flag) {
		std::cout << "创建socket失败" << std::endl;
	} else {
		return;
	}

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
	int ret = WSAStartup(0x0202, &wsa);
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

	std::thread t_accpet(&Server::run, this);
	t_accpet.detach();
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
		char buffer[DATA_UNIT * 4] = { 0 };
		string transBuffer;
		int ret = recv(c, buffer, DATA_UNIT * 4, 0);
		if (ret > 0) {
			m_mapClient[c] = heartCount;
			transBuffer.append(buffer).append("    ");
			transBuffer.append("from: ").append(to_string(c));
			
			// 发送到每个客户端上 转发作用 除了自己
			ShareMessage(c, transBuffer);
		}
	}
}

bool Server::ShareMessage(SOCKET c, std::string &message)
{
	if (c <= 0) {
		cout << "Server::ShareMessage  para  error!" << endl;
		return false;
	}
	auto iter = m_mapClient.begin();
	for (; iter != m_mapClient.end(); ++iter) {
		if ((*iter).first == c)
			continue;
		message.append("    ").append(getLocalTime());
		int ret = send((*iter).first, (char *)message.c_str(), message.length(), 0);
		if (ret != message.length())
			cout << " 转发送失败  " << c << endl;
	}
	return false;
}

std::string Server::getLocalTime()
{
	time_t rawtime;
	struct tm *info;
	char buffer[80];

	time(&rawtime);

	info = localtime(&rawtime);

	strftime(buffer, 80, "%Y-%m-%d %H:%M::%S", info);//以年月日_时分秒的形式表示当前时间

	return buffer;
}
