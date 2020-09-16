#pragma once
#include "MySQLInterface.h"
#include "data.h"

#include <string>
#include <sstream>
#include <vector>
#include<filesystem>

#include "tinyxml/tinystr.h"
#include "tinyxml/tinyxml.h"

using namespace std;


class MysqlPacked 
{
public:
	MysqlPacked();
	virtual ~MysqlPacked();

	void selectUserInfo(const UserInfo &userinfo,UserInfoReply &);
	UserInfoReply registerUserInfo(UserInfo& userinfo);
	int getUserId(string userName);
	void selectRegisterUserInfo(RegisterInfo& registerinfo, registerInfoReply &infoRet);
	UpFileReply insertFile(UpFile& fileinfo);
	DownFileReply selectFile(DownFile &fileinfo);
	SelectFilesReply* selectFromFilePath(SelectFiles &fileinfo);
	LogStruct* logRequestData(LogRequestStruct &info);
	LogUserId* requestLogWidUserId();
	string getLogSql(int userId,int level,int sortStatus);
	bool createPath(CreatePathStruct &info);
	string getTime();
	bool selectFileExist(std::string filepath ,std::string table);

	bool addOneFileToSql(UpFileRequest &,std::string table);

	bool updataFileInfo(UpFileReply,std::string table);
	ShareCodeReply* getShareCodeInfo();
	void saveShareCode(ShareCodeReq &);
	
	bool uploadCompleted(UpFileReply &info);
	
	
	string getNowTime();
	int getFileSize(std::string filePath);
	string getFileName(std::string filepath);
	FileType getFileType(std::string filePath);
	bool isDir(std::string filePath);
	bool deleteFile(int fileId,int userId);
	bool recycleDeleteFile(int fileId);
	bool cycleRecycleFile(int fileid);
	SelectFilesReply* recycleRequestData(int userId);
	bool loadConfig();
	DownFileRequestReply downloadFileInfo(int fileId);
private:
	MySQLInterface *db;
	MySQLConInfo config;
/*	friend DataManager;*/
};

