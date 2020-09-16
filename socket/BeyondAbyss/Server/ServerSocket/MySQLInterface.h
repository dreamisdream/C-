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


#define ERROR_QUERY_FAIL -1 // 操作失败


// 定义MySQL连接信息
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
	void SetMySQLConInfo(char* server, char* username, char* password, char* database, int port);// 设置连接信息

	void init();
	bool Open();  // 打开连接
	void Close(); // 关闭连接
	bool Select(const std::string& Querystr, std::vector<std::vector<std::string> >& data);	     // 读取数据
	bool Query(const std::string& Querystr);     // 其他操作
	int GetInsertID(const std::string& Querystr);// 插入并获取插入的ID,针对自动递增ID
	void ErrorIntoMySQL();		 // 错误消息

public:
	int ErrorNum;				 // 错误代号  
	const char* ErrorInfo;       // 错误提示  

private:
	MySQLConInfo MysqlConInfo;	 // 连接信息
	MYSQL MysqlInstance;		 // MySQL对象
	MYSQL_RES *Result;			 // 用于存放结果 
};

#endif 