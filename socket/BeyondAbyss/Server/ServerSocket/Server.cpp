#include "stdafx.h"
#include "Server.h"

//#include <sys/epoll.h>
#include <iostream>
#include <thread>
#include "data.h"
using namespace std;

Server::Server()
{
	flag = 1;
	thread heart_check_thread(&Server::heartCheck, this);
	heart_check_thread.detach();
	init();
}

Server::~Server()
{
	
	for (auto it = dataManagerSocket.begin(); it != dataManagerSocket.end(); ++it)
	{
		delete it->second;
		it->second = nullptr;
	}
	for (auto it = m_heart_check.begin(); it != m_heart_check.end(); ++it)
		closesocket(it->first);
	closesocket(m_server);
	WSACleanup();
}

void Server::run()
{
	if (!flag) {
		cout << "����������ʧ��run" << endl;
		return;
	}
		
	while (true)
	{
		SOCKADDR_IN addr_client;
		int len_addr_client = sizeof(addr_client);
		cout << "�ȴ�����"<< endl;
		m_client = accept(m_server, (sockaddr*)&addr_client, &len_addr_client);
		if(!dealError(m_client))
			continue;
		if (m_client <= 0)
			continue;
		m_heart_check[m_client] = MAX_LINKER;
		DataManager * dataManager = new DataManager(m_client);
		dataManagerSocket[m_client] = dataManager;
		cout << "�ͻ��ˣ�" << m_client << "���ӳɹ�" << endl;
		thread recv_thread(&Server::recvData, this, m_client);
		recv_thread.detach();
	}
}

void Server::init()
{
	WSADATA wsa;
	int wsa_ret = WSAStartup(MAKEWORD(2, 2), &wsa);
	if (wsa_ret != 0) {
		PRINT_ERROR("��ʼ������ʧ��!");
		flag = 0;
		return;
	}
	m_server = socket(AF_INET, SOCK_STREAM, 0);
	if (m_server == INVALID_SOCKET) {
		PRINT_ERROR("����������ʧ��!");
		flag = 0;
		return;
	}
	SOCKADDR_IN addr_server;
	addr_server.sin_family = AF_INET;
	addr_server.sin_port = htons(8888);
	addr_server.sin_addr.S_un.S_addr = inet_addr("0.0.0.0");
	int bind_ret = ::bind(m_server, (sockaddr*)&addr_server, sizeof(addr_server));
	if (bind_ret == -1) {
		PRINT_ERROR("bindʧ��");
		flag = 0;
		return;
	}
	int listen_ret = listen(m_server, MAX_LINKER);
	if (listen_ret == -1) {
		PRINT_ERROR("listenʧ��!");
		flag = 0;
		return;
	}
}

void Server::recvData(SOCKET c)
{
	while (true)
	{
		char buf_head[HEAD_LEN] = { 0 };
		int recv_head_ret = recv(c, buf_head, HEAD_LEN, 0);
		if (!dealError(recv_head_ret)) 
		{
			closesocket(c);
			break;
		}
		m_heart_check[c] = MAX_LINKER;
		PRINT_NORMAL(recv_head_ret);
		if (recv_head_ret == HEAD_LEN)
		{
			int len = *(int *)(buf_head + 4);
			int type = *(int *)(buf_head);
			if (len > 2000) 
			{
				PRINT_ERROR("���͵Ľṹ�峤�ȴ���2000");
				continue;
			}
			char *data = (char*)malloc(len);
			memset(data, 0, len);
			memcpy(data, buf_head, HEAD_LEN);
			int data_len = recv(c, data + HEAD_LEN, len - HEAD_LEN, 0);
			if (!dealError(data_len, data))
			{
				closesocket(c);
				free(data);
				data = nullptr;
				break;
			}
			if (data_len == (len - HEAD_LEN))
			{
				dataManagerSocket[c]->addData(type, data);
				free(data);
				data = nullptr;
			}
			free(data);
			data = nullptr;
		}else
			break;
	}
	
}

bool Server::dealError(int num ,char *data)
{
	if (num == 0)
	{
		PRINT_ERROR("�������ر�");
		if (data == nullptr)
			return false;
		free(data);
		data = nullptr;
		return false;
	}
	else if (num == -1)
	{
		PRINT_ERROR("�쳣�ر�");
		if (data == nullptr)
			return false;
		free(data);
		data = nullptr;
		return false;
	}
	return true;
}

void Server::heartCheck()
{
	cout << "heartCheck" << endl;
	while (true)
	{
		for (auto it = m_heart_check.begin(); it != m_heart_check.end();)
		{
			cout << "�ͻ���:" << it->first << "ʣ��ʱ��:" << it->second << endl;
			it->second--;
			if (it->second == 0)
			{
				cout << "�ͻ���:" << it->first << "���ӹر�" << endl;
				closesocket(it->first);
				m_heart_check.erase(it++);
			}
			else
				it++;
		}
		Sleep(2000);
	}
}
