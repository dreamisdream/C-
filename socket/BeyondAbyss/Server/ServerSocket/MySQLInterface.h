#pragma once

#ifndef __MYSQL_INTERFACE_H__  
#define __MYSQL_INTERFACE_H__  

#include <WinSock2.h>
#include <string>
#include <vector>  
#include <map>
#include <iostream>
#include <mutex>

#include "MySQL/include/mysql.h"

#pragma comment(lib, "ws2_32.lib")  
#pragma comment(lib, "MySQL/lib/libmysql.lib")

using namespace std;


#define ERROR_QUERY_FAIL -1 // ����ʧ��


// ����MySQL������Ϣ
typedef struct
{
	char* server;
	char* user;
	char* password;
	char* database;
	int port;
}MySQLConInfo;

class MySQLInterface
{
public:
	MySQLInterface() {}
	MySQLInterface(const MySQLInterface& object) {}
	void operator=(const MySQLInterface& object) {}
	virtual ~MySQLInterface();
	static MySQLInterface* m_object;

public:
	static MySQLInterface* getInstance();
	void SetMySQLConInfo(char* server, char* username, char* password, char* database, int port);// ����������Ϣ

	void init();
	bool Open();  // ������
	void Close(); // �ر�����
	bool Select(const std::string& Querystr, std::vector<std::vector<std::string> >& data);	     // ��ȡ����
	bool Query(const std::string& Querystr);     // ��������
	int GetInsertID(const std::string& Querystr);// ���벢��ȡ�����ID,����Զ�����ID
	void ErrorIntoMySQL();		 // ������Ϣ

public:
	int ErrorNum;				 // �������  
	const char* ErrorInfo;       // ������ʾ  

private:
	MySQLConInfo MysqlConInfo;	 // ������Ϣ
	MYSQL MysqlInstance;		 // MySQL����
	MYSQL_RES *Result;			 // ���ڴ�Ž�� 
};

#endif 