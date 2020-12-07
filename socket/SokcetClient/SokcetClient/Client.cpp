#include "stdafx.h"
#include "Client.h"
#include "common.h"

#include <thread>
#include <string>
#include <iostream>
using namespace std;
Client::Client()
{
	init();
}

Client::~Client()
{
	closesocket(client);
	WSACleanup();
}

void Client::init()
{
	WSADATA wsa;
	WSAStartup(0x0202, &wsa);
	
	client = socket(AF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(c_port);
	addr.sin_addr.S_un.S_addr = inet_addr(s_ip);
	int ret = connect(client, (sockaddr*)&addr, sizeof(addr));
	if (ret == 0) {
		thread t_recv(&Client::readMessage, this, client);
		t_recv.detach();
		sendMessage();
	}
	else
		cout << "connect ret:" << ret << endl;	
}

void Client::sendMessage()
{
	cout << "*****服务端连接成功*****" << endl;
	string str;
	while (cin >> str) {
		int ret = send(client, (char *)str.c_str(), str.length(), 0);
		if (ret != str.length())
			cout << "发送失败" << endl;
	}
}

void Client::readMessage(SOCKET c)
{
	cout << "Client::readMessage thread load" << endl;
	while (true) {
		char buffer[DATA_UNIT * 4] = { 0 };
		int ret = recv(c, buffer, DATA_UNIT * 4, 0);
		if (ret < 0)
			return;
		cout << buffer << endl;
	}
}
