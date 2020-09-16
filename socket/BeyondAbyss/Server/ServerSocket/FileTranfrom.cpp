#include "stdafx.h"
#include "FileTranfrom.h"
#include "data.h"


#include <iostream>
#include <thread>
#include <fstream>

using namespace std;

FileTranfrom::FileTranfrom(const string filepath, int userId,bool upload)
	: m_filepath(filepath)
	, m_upload(upload)
	, sql(new MysqlPacked)
	,useId(useId)
{
	init();
	cout << "(\"????socket?????$)A!B????\")" << endl;
	thread t_heart(&FileTranfrom::heartCheck, this);
	t_heart.detach();
}


FileTranfrom::~FileTranfrom()
{
	closesocket(m_hListen);
	delete sql;
	sql = nullptr;
}

int FileTranfrom::getPort()
{
	return ntohs(addr_server.sin_port);
}

void FileTranfrom::start()
{
	PRINT_NORMAL("??????????$)A!A((??????");
	while (true) {
		SOCKADDR_IN addr = { 0 };
		int len = sizeof(SOCKADDR_IN);
		// $)A!A((??
		SOCKET client = accept(m_hListen, (SOCKADDR*)&addr, &len);
		std::cout << "??????????????client:" << client << "?$)A(*?(.??????" << WSAGetLastError() << endl;
		if (client == INVALID_SOCKET) {
			cout << "accpet:?$)A!l!c?:" << client << endl;
			//closesocket(client);
			//client = INVALID_SOCKET;
			break;
		}
		m_file_heart[client] = MAX_LINKER;

		thread threadRecvFile(&FileTranfrom::recvFile, this, client);
		threadRecvFile.detach();
	
	}
}

bool FileTranfrom::dealUploadPack(UpFile &)
{
	return false;
}

bool FileTranfrom::dealDownloadPack(DownFile &info, SOCKET c)
{
	ifstream fp(info.filePath, ios::binary | ios::in);
	int recvBytes = 0;
	int allBytes = 0;
	if (!fp.is_open())
		return false;
	else
	{
		fp.seekg(0, fp.end);
		allBytes = fp.tellg();
		fp.seekg(0, fp.beg);
	}
	cout << "????$)A!A??(.????" << allBytes << endl;
// 	while (true){
// 		DownFileReply sendInfo;
// 		sendInfo.success = 1;
// 		fp.read(sendInfo.file_content, sizeof(sendInfo.file_content));
// 		recvBytes = fp.tellg();
// 		int sendLen = send(c, (char*)&sendInfo, sendInfo.head.len, 0);
// 		if (sendLen <= 0) {
// 			cout << "sendLen:" << sendLen << endl;
// 			break;
// 		}
// 		if (recvBytes >= allBytes)
// 			break;
// 	}

	while (!fp.eof())
	{
		DownFileReply package;
		package.success = 1;
		fp >> package.file_content;
		recvBytes = fp.tellg();
		int sendLen = send(c, (char*)&package, package.head.len, 0);

		if (sendLen <= 0)
		{
			std::cout << "$)A7"KMJ'0\#,4mNsBk#:"<< WSAGetLastError();
			break;
		}
	}
	return true;
}

bool FileTranfrom::init()
{
	m_hListen = socket(AF_INET, SOCK_STREAM, 0);
	addr_server.sin_family = AF_INET;
	addr_server.sin_port = htons(9999);
	addr_server.sin_addr.S_un.S_addr = inet_addr("0.0.0.0");
	int bind_ret = _WINSOCK2API_::bind(m_hListen, (sockaddr*)&addr_server, sizeof(addr_server));
	int retListen =listen(m_hListen, 5);
	cout << "m_hListen:"<<m_hListen <<"   bind :"<< bind_ret <<"  retLisen:" << retListen << endl;
	
	thread t1(&FileTranfrom::start, this);
	t1.detach();
	return false;
}
// ???$)A!l????????
void FileTranfrom::sendFile(SOCKET c)
{
	// ???????$)A(.??????;
	while (true)
	{
		// ????????
		
		// $)A!@?????????
		// ????
	}
	//closesocket(c);
}
// ???$)A!l????????
void FileTranfrom::recvFile(SOCKET c)
{
	string suffix = "C:\\Users\\Administrator\\Desktop\\Server\\Debug\\upload\\";
	__int64 recvBytes = 0;
	// ???????$)A(.??????
	ofstream file;
	while (true)
	{
	//	std::cout << "$)A!A?!@?????????!c(9:" << c <<endl;
		char head[HEAD_LEN] = { 0 };
		int ret = recv(c, head, HEAD_LEN, 0);
	//	std::cout << "?????$)A!h????" << ret << "--" <<"?(*?(.????"<< WSAGetLastError() << endl;
		if (ret <= 0) {
			//closesocket(c);
			PRINT_ERROR("?????????????$)A!l!c?");
			break;
		}
		int len = *(int *)(head + 4);
		int type = *(int *)(head );
		if (len <= 0)
		{
			cout << "len?$)A!h???????!';;" << len <<"  type:"<<type<< endl;
			break;
		}
		char *data = (char *)malloc(len);
		memset(data, 0, len);
		memcpy(data, head, HEAD_LEN);
	//	std::cout <<"type:"<< type << "--" <<"len:"<< len << endl;
		
		int allRet = recv(c, data + HEAD_LEN, len - HEAD_LEN, 0);
	//	cout << "allRet:" << allRet << endl;
		int recvLen = allRet+HEAD_LEN;
		while (recvLen < len  ) {
			allRet = recv(c, data + recvLen, len - recvLen, 0);
			if (allRet <=0)
				break;
			recvLen += allRet;
			cout << "    allRet:" << allRet << " RecvLen:" << recvLen << endl;
		}
		if (allRet <= 0) {
	//		closesocket(c);
			PRINT_ERROR("?????????????$)A!l!c?");
			break;
		}
		cout << "????$)A!c(9??type??" << type << endl;
		if (type == FileInfoType::UP_FILE){
			;
		}else if (type == FileInfoType::DOWN_FILE){
			cout << "???????????????$)A!B????" << endl;
 			DownFile *info = (DownFile*)data;
 			thread downloadThread(&FileTranfrom::dealDownloadPack,this,*(DownFile*)data,c);
 			downloadThread.detach();
			break;
			}
		UpFile *fi = (UpFile *)data;
		recvBytes += fi->fileContentSize;
	//	std::cout << "recvBytes:" << recvBytes <<" file_contentsize:"<<fi->fileContentSize<<"---"<<fi->fileSize<< endl;	
		if (file.is_open())
		{
			file.write(fi->fileContent, fi->fileContentSize);
			file.flush();
	//		cout << "?????????$)A(0?? ??????"<< suffix + fi->fileName << endl;
		}
		else
		{
			file.open(suffix + fi->fileName, ios::binary | ios::out);
			file.write(fi->fileContent, fi->fileContentSize);
			file.flush();
			cout << "?????????$)A(0?? ??????"<< suffix + fi->fileName << endl;
		}
		// ???????$)A!l?? 
		UpFileReply reply;
		reply.success = 1;
		reply.head.type = FileInfoType::UP_FILE_REPLY;
		reply.head.len = sizeof(UpFileReply);
		reply.recvBytes = recvBytes;
		reply.totalSize = fi->fileSize;
		reply.fileId = fi->fileId;
		reply.userId = this->useId;
		memcpy(reply.filePath, fi->filePath, sizeof(reply.filePath));
		cout << "??????????????" << reply.filePath << endl;
		cout <<"????????????????userId??"<< reply.userId << endl;
		sql->updataFileInfo(reply, "t_upload");
		int retSend = send(c, (char*)&reply, reply.head.len, 0);
		std::cout << "retSend:" << retSend << endl;
		m_file_heart[c] = MAX_LINKER;
//
		if (fi->fileSize == recvBytes) {
			cout << "?????????$)A(:??..." << fi->fileName << endl;
			//sql->addOneFileToSql();
			sql->uploadCompleted(reply);
			free(data);
			data = nullptr;
			break;
		}
		free(data);
		data = nullptr;
	}
//	closesocket(c);
}


void FileTranfrom::heartCheck()
{
	while (true)
	{
		for (auto it = m_file_heart.begin(); it != m_file_heart.end();)
		{
			cout << "[      ????????????:" << it->first << "???$)A($?!@??:" << it->second <<"        ]"<< endl;
			it->second--;
			if (it->second == 0)
			{
				cout << "????????????:" << it->first << "??????$)A!@?" << endl;
				closesocket(it->first);
				m_file_heart.erase(it++);
			}
			else
				it++;
		}
		Sleep(2000);
	}
}
