#pragma once

#include "MysqlPacked.h"
#include <string>
#include <WinSock2.h>
#include <map>
#include "data.h"

class FileTranfrom
{
public:
	FileTranfrom(const std::string filepath,int userId,bool upload);
	~FileTranfrom();
	int getPort();
	bool init();
private:
	void sendFile(SOCKET c);
	void recvFile(SOCKET c);
	void heartCheck();
	void start();

	bool dealUploadPack(UpFile &);
	bool dealDownloadPack(DownFile &,SOCKET c);

	int m_hListen;
	std::string m_filepath;
	bool m_upload;
	MysqlPacked *sql;
	SOCKADDR_IN addr_server;
	int useId;
	std::map<SOCKET, int> m_file_heart;
};

