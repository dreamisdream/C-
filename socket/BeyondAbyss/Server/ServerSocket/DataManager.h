#pragma once

#include "MysqlPacked.h"
#include <string>
#include "data.h"
#include "Logging.h"
#include "FileTranfrom.h"
#include<filesystem>
#include <string>
#include <map>

using namespace std;
using namespace std::tr2::sys;

class DataManager
{
public:
	DataManager(SOCKET client);
	DataManager(int type,char *buf,int len);
	~DataManager();
	void  addData(int type, char *buf);
	void  login(UserInfo &);
	void  registerUser(RegisterInfo &);
	void createUserPath(string path);
	void  selectFile(DownFile &);
	void  upFile(UpFile &);
	void  selectPath(SelectFiles &);
	void selectLog(LogRequestStruct &);
	void deleteFile(DeleteFileStruct &);
	bool  selectExist(std::string filepath, std::string table);
	bool  addOneFile(std::string filepath, std::string table);
	void recycleDeleteFile(RecycleDeleteFileStruct &);
	void recycleRecoverFile(RecyRecoverDataStruct &);//�ָ�����վ����recycleRecoverFile
	void recycleRequestData(RequestRecyleDataStruct &);
	void requestLogWidUserId(LogUserId &);
	void requestCreatePath(CreatePathStruct &);
	void createPath(string newPath);
	// �����ļ�����
	void getAllShareFileInfo();
	void upShareInfo(ShareCodeReq &);
	void ShareInfoGetDownLoad(ShareCodeDownLoad &);
		// �ϴ�����
	void upFileReq(UpFileRequest &);
	// ��������
	void downFileReq(DownFileRequest &);
	string  getNowTime();
	void checkPath(string str);
private:
	MysqlPacked *sqlDeal;
	SOCKET client;
	Logging *log;
	static FileTranfrom *fileTransfrom;
};

