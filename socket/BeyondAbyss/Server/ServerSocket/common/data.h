#pragma once
// 用户登录 注册
#include <iostream>
#include <QString>

using namespace std;

struct FileInfoStruct;

// 用户权限
struct UserPermission
{
	int admin;
	int isDelete;
	int down;
	int up;
	int share;
	int log;
	UserPermission() :admin(0), isDelete(0), down(1), up(1), share(1), log(0) {};
};
// 文件操作类型
enum FileInfoType
{
	LOGIN = 1,
	REGISTER = 2,
	LOGIN_REPLY = 3,
	REGISTER_REPLY = 4,
	UP_FILE = 5,
	UP_FILE_REPLY = 6,
	DELETE_FILE = 20,
	DELETE_FILE_REPLY =21,
	DOWN_FILE = 30,
	DOWN_FILE_REPLY = 31,
	SELECT_FILE = 40,
	SELECT_FILES = 41,
	SELECT_FILE_REPLY = 50,
	SELECT_FILES_REPLY = 51,
	SHARE = 55,

	ERROR_CODE = 400
};
// 用户登录注册信息
struct UserInfo
{
	FileInfoType type;
	int len;
	char userName[64];
	char password[24];
	char time[24];
	UserPermission userPermission;
};

struct UserInfoReply
{
	FileInfoType type;
	int len;
	int success; // 0失败 1成功
	int userId;
	UserPermission permission;
};

// 文件类型
enum FileType
{
	TXT = 1001,
	PNG = 1300
};

// 文件传输头
struct FileHeadInfo
{
	FileInfoType type;
	int len;
	int userId;
};

// 客户端上传文件  
struct UpFile
{
	FileHeadInfo head;
	char filePath[128];
	char fileSize[24];
	char fileName[128];
	int fileType;
	int isFile;
    int nowTime;
    int orders;
    int fileContentSize;
	char fileContent[1024];
	int confirm;
	UpFile()
	{
		head.type = FileInfoType::UP_FILE;
		head.len = sizeof(UpFile);
	}
};
// 服务器下载文件
struct UpFileReply
{
	FileHeadInfo head;
	int success;
	UpFileReply()
	{
		head.type = FileInfoType::UP_FILE;
		head.len = sizeof(UpFileReply);
		success = 0;
	}
};

// 下载文件
struct DownFile
{
	FileHeadInfo head;
	char filePath[128];
	DownFile()
	{
		head.type = FileInfoType::UP_FILE;
		head.len = sizeof(DownFile);
	}
};
struct DownFileReply
{
	FileHeadInfo head;
	int success;
	char filePath[128];
	char fileSize[24];
	char fileName[128];
	int fileType;
	int isFile;
	int now_time;
	char file_content[1024];
	int confirm;
	DownFileReply()
	{
		head.type = FileInfoType::UP_FILE;
		head.len = sizeof(DownFile);
		success = 0;
	}
};

struct DeleteFile
{
	FileHeadInfo head;
	char file_path[128];
};
struct DeleteFileReply
{
	FileHeadInfo head;
	int isDelete;
};
// 获取path目录下的所有文件请求
struct SelectFiles
{
	FileHeadInfo head;
	char path[128];
	SelectFiles()
	{
		head.type = FileInfoType::UP_FILE;
		head.len = sizeof(SelectFiles);
	}
};
struct DeleteFileStruct
{
	FileHeadInfo head;
	char file_path[128];
};
struct DeleteFileReply
{
	FileHeadInfo head;
	int isDelete;
};
/*
struct FileInfoStruct
{
	char filePath[128];
	char fileSize[24];
	char fileName[128];
	int fileType;
	int isFile;
	int nowTime;
    FileInfoStruct() {}
};
*/
struct FileInfoStruct
{
    char filePath[128];
    char fileSize[24];
    char fileName[128];
    int fileId;
    int fileType;
    int isFile;
    int nowTime;
    FileInfoStruct()
    {
        isFile =1;
        fileType = 1;
        nowTime = 202005;
        fileId = 102;
        strcpy_s(fileName,sizeof("平凡的世界.txt"),"平凡的世界.txt");
        strcpy_s(fileSize,sizeof("1.5MB"),"1.5MB");
        strcpy_s(filePath,sizeof("网盘根目录/电子书"),"网盘根目录/电子书");
    }
};

// 选择目录回复
struct SelectFilesReply
{
	FileHeadInfo head;
	int sucess;
	int len;
    //FileInfoStruct file[0];
	SelectFilesReply()
	{
		head.type = FileInfoType::UP_FILE;
		head.len = sizeof(SelectFilesReply);
        len = sizeof(FileInfoStruct);
		sucess = 0;
	}
};


// 
// 回复单个文件信息请求
struct SelectFileReply
{
	FileHeadInfo head;
	char file_path[128];
	char file_name[128];
	char file_size[24];
	FileType file_type;
	char last_updata_time[24];
	int is_file;  // 0 为目录 1为文件
};

// 错误信息
enum ServerError
{
	FILE_EXIST = 401,
	FILE_NO_EXIST = 402
};

#define HEAD_LEN 8
#define PRINT_ERROR(x) cout <<"[  "<< (x)<<":"<<WSAGetLastError() <<" "<<__FUNCTION__<<" "<<__LINE__<<"  ]"<<endl
#define PRINT_NORMAL(x) cout <<"[              " << (x) <<"                    ]"<<endl
