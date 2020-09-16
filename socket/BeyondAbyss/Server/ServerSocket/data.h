#pragma once
// 用户登录 注册
#include <iostream>

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
    UP_FILE_REQUEST,
    UP_FILE_REQUEST_REPLY,
    DOWN_FILE_REQUEST,
    DOWN_FILE_REQUEST_REPLY,
	DELETE_FILE = 20,
	DELETE_FILE_REPLY =21,
	DOWN_FILE = 30,
	DOWN_FILE_REPLY = 31,
	SELECT_FILE = 40,
	SELECT_FILES = 41,
	SELECT_FILE_REPLY = 50,
	SELECT_FILES_REPLY = 51,
	SHARE = 55,
    SHARE_REPLY,
	SHARE_CODE_INFO,
	SHARE_INFOS,
	SHARE_DOWNLOAD_FILE,
	SHARE_DOWNLOAD_REPLY,
	RECYCLE_DELETE_REQUEST = 61,
	RECYCLE_DELETE_REPLE = 62,
	RECYCLE_REQUEST_DATA = 63,
	RECYCLE_RESPONSE_DATA = 64,
	RECYCLE_RECOVER_REQUEST = 65,
	RECYCLE_RECOVER_REPLE = 66,
	CRETE_NEW_PATH = 70,
	UPDATE_WIDGET_INFO = 71,
    LOG_REQUEST = 80,
    LOG_ALLINFO = 81,
	LOG_ALLUSER_ID = 82,
	HEART_PACK,
    ERROR_CODE = 400
};
// 用户登录注册信息
struct UserInfo
{
	FileInfoType type;
	int len;
	char userName[64];
	char password[64];
	char time[24];
	UserPermission userPermission;
};

struct RegisterInfo
{
	FileInfoType type;
	int len;
	char userName[64];
	char password[64];
	char identityCode[64];
	char phoneNumber[64];
	char Email[64];
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

struct registerInfoReply
{
	FileInfoType type;
	int len;
	int success; // 0失败 1成功
	int userId;
	UserPermission permission;
	registerInfoReply()
	{
		type = FileInfoType::REGISTER_REPLY;
		len = sizeof(registerInfoReply);

	}
};

// 文件类型
enum FileType
{
    DIR = 0,
    PNG = 1,
    TEXT = 2,
    MUSIC = 3,
    VIDEO = 4,
    EXCEL = 5,
    RAR = 6,
    PDF = 7,
    PPT = 8,
    DOC = 9,
    OTHER = 10
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
    __int64 fileSize;
	char fileName[128];
	int fileId;
	int fileType;
	int isFile;
    int nowTime;
    int orders;
    int fileContentSize;
    char fileContent[10*1024];
	int confirm;
	int userId;
	UpFile()
    {
        memset(filePath,0,sizeof(filePath));
        memset(fileName,0,sizeof(fileName));
        memset(fileContent,0,sizeof(fileContent));
		head.type = FileInfoType::UP_FILE;
		head.len = sizeof(UpFile);
	}
};
// 服务器下载文件
struct UpFileReply
{
	FileHeadInfo head;
    __int64 totalSize;
	int success;
    __int64 recvBytes;
	int fileId;
	int userId;
	char filePath[128];
	UpFileReply()
	{
		memset(filePath, 0, sizeof(filePath));
		head.type = FileInfoType::UP_FILE_REPLY;
		head.len = sizeof(UpFileReply);
		success = 0;
	}
};

// 下载文件
struct DownFile
{
	FileHeadInfo head;
	char filePath[128];
	int fileId;
	char fileName[128];
	__int64 filePointer;
	DownFile()
	{
		memset(filePath, 0, sizeof(filePath));
		memset(fileName, 0, sizeof(fileName));
		head.type = FileInfoType::DOWN_FILE;
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
		head.type = FileInfoType::DOWN_FILE_REPLY;
		head.len = sizeof(DownFileReply);
		success = 0;
	}
};

struct DeleteFileStruct
{
	FileHeadInfo head;
	int userId;
	int fileId;
	DeleteFileStruct()
	{
		head.type = FileInfoType::DELETE_FILE;
		head.len = sizeof(DeleteFileStruct);
	}
};


struct DeleteFileReply
{
	FileHeadInfo head;
	int isDelete;
	int fileId;
	DeleteFileReply()
	{
		head.type = FileInfoType::DELETE_FILE_REPLY;
		head.len = sizeof(DeleteFileReply);
	}
};
// 获取path目录下的所有文件请求
struct SelectFiles
{
	FileHeadInfo head;
	char path[128];
	SelectFiles()
	{
		head.type = FileInfoType::SELECT_FILES;
		head.len = sizeof(SelectFiles);
	}
};

//文件信息结构体
struct FileInfoStruct
{
	int fileId;
	int fileType;
	int isFile;
	long long fileSize;
	char nowTime[32];
	char fileName[128];
	char filePath[128];
	char deleteTime[32];
	FileInfoStruct()
	{
		isFile = 1;
		fileType = 1;
		fileId = 102;
		fileSize = 12012540201;
		strcpy_s(nowTime, sizeof("2020-05-31 22:04:22"), "2020-05-31 22:04:22");
		strcpy_s(fileName, sizeof("平凡的世界.txt"), "平凡的世界.txt");
		strcpy_s(filePath, sizeof("网盘根目录/电子书"), "网盘根目录/电子书");
	}
};

// 选择目录回复(文件列表不定长结构体)
struct SelectFilesReply
{
	FileHeadInfo head;
	int sucess;
	int len;
	int file_status;
    FileInfoStruct file[0];
	SelectFilesReply()
	{
		head.type = FileInfoType::SELECT_FILES_REPLY;
		head.len = sizeof(SelectFilesReply);
        len = sizeof(FileInfoStruct);
		sucess = 0;
		file_status = 1;
	}
};


//请求回收站数据
struct RequestRecyleDataStruct
{
	FileHeadInfo head;
	RequestRecyleDataStruct()
	{
		head.type = FileInfoType::RECYCLE_REQUEST_DATA;
		head.len = sizeof(RequestRecyleDataStruct);
	}
};

//响应回收站数据
struct ResponseRecyleDataStruct
{
	FileHeadInfo head;
	ResponseRecyleDataStruct()
	{
		head.type = FileInfoType::RECYCLE_RESPONSE_DATA;
		head.len = sizeof(ResponseRecyleDataStruct);
	}
};


//回收站彻底删除 haoyutong
struct RecycleDeleteFileStruct
{
	FileHeadInfo head;
	int fileId;
	RecycleDeleteFileStruct()
	{
		head.type = FileInfoType::RECYCLE_DELETE_REQUEST;
		head.len = sizeof(RecycleDeleteFileStruct);
	}
};

//回收站彻底删除服务器回复 haoyutong
struct RecycleDeleteFileReply
{
	FileHeadInfo head;
	int fileId;
	int succeed;
	RecycleDeleteFileReply()
	{
		head.type = FileInfoType::RECYCLE_DELETE_REPLE;
		head.len = sizeof(RecycleDeleteFileReply);
	}
};


//回收站恢复删除数据结构体
struct RecyRecoverDataStruct
{
	FileHeadInfo head;
	int fileid;
	RecyRecoverDataStruct()
	{
		head.type = RECYCLE_RECOVER_REQUEST;
		head.len = sizeof(RecyRecoverDataStruct);
	}
};

//回复回收站恢复删除数据结构体
struct RecycleRecoverFileReply
{
	FileHeadInfo head;
	int fileId;
	int succeed;
	RecycleRecoverFileReply()
	{
		head.type = FileInfoType::RECYCLE_RECOVER_REPLE;
		head.len = sizeof(RecycleRecoverFileReply);
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
//
struct UpFileRequest
{
    FileInfoType type;
    int len;
    int fileId;
    char filePath[128];
    char fileName[128];
    __int64 fileSize;
	int userId;
    UpFileRequest()
    {
        type = FileInfoType::UP_FILE_REQUEST;
        len = sizeof(UpFileRequest);
    }
};

struct UpFileRequestReply
{
    FileInfoType type;
    int len;
	char filePath[128];
    int fileId;
	int port;
	int userId;
    UpFileRequestReply()
    {
        type = FileInfoType::UP_FILE_REQUEST_REPLY;
        len = sizeof(UpFileRequestReply);
    }
};

struct DownFileRequest
{
	FileInfoType type;
	int len;
	int fileId;
	// 	char filePath[128];
	char fileName[128];
	/*	__int64 fileSize;*/
	int userId;
	DownFileRequest()
	{
		type = FileInfoType::DOWN_FILE_REQUEST;
		len = sizeof(DownFileRequest);
	}
};

struct DownFileRequestReply
{
	FileInfoType type;
	int len;
	int fileId;
	char filePath[128];
	__int64 fileSize;
	__int64 filePointer;
	int port;
	int userId;
	DownFileRequestReply() {
		type = FileInfoType::DOWN_FILE_REQUEST_REPLY;
		len = sizeof(DownFileRequestReply);
	}
};


// 错误信息
enum ServerError
{
	FILE_EXIST = 401,
	FILE_NO_EXIST = 402
};

//日志信息结构体
struct LogInfo
{
    int id;
    int user_id;
    int level;
    char log_time[32];
    char log_info[128];
    LogInfo()
    {
        memset(log_time,0,32);
        memset(log_info,0,128);
        id = 1;
        user_id =1;
        level = 1;
        memcpy(log_time,"2020-06-04 21:25:29",sizeof("2020-06-04 21:25:29"));
        memcpy(log_info,"客户端程序启动",sizeof("客户端程序启动"));
    }
};

//日志不定长
struct LogStruct
{
    int type;
    int len;
    int success;
    int count;
    LogInfo log[0];
    LogStruct()
    {
        success = 0;
        type = FileInfoType::LOG_ALLINFO;
        len = sizeof(LogStruct);
    }
};

//日志请求结构体
struct LogRequestStruct
{
    FileHeadInfo head;
    int selectUserId;
    int logLevle;
    int sortStatus;
    LogRequestStruct()
    {
        head.type = FileInfoType::LOG_REQUEST;
        head.len = sizeof(LogRequestStruct);
    }
};


struct LogUserId
{
	int type;
	int len;
	int success;
	int count;
	int allId[0];
	LogUserId()
	{
		type = FileInfoType::LOG_ALLUSER_ID;
		len = sizeof(LogUserId);
		success = 0;
		count = 0;
	}
};


//create a new path struct
//dong
struct CreatePathStruct
{
	int type;
	int len;
	int success;
	int userId;
	char newPath[256];
	char path[256];
	CreatePathStruct()
	{
		memset(newPath, 0, 256);
		memset(path, 0, 256);
		type = FileInfoType::CRETE_NEW_PATH;
		len = sizeof(CreatePathStruct);
		success = 0;
	}
};

//通知客户端更新界面统一接口
struct UpDateFilelist
{
	int type;
	int len;
	int widgetType;
	UpDateFilelist()
	{
		type = FileInfoType::UPDATE_WIDGET_INFO;
		len = sizeof(UpDateFilelist);
		widgetType = 1;
	}
};

struct ShareCodeReq
{
	FileHeadInfo head;
	int fileId;
	char code[4];
	ShareCodeReq() {
		head.type = FileInfoType::SHARE;
		head.len = sizeof(ShareCodeReq);
	}
};

struct CodeInfo {
	int len;
	int userId;
	int fileId;
	char code[5];
	CodeInfo() {
		len = sizeof(CodeInfo);
		memset(code, 0, 5);
	}
};

struct ShareCodeReply
{
	FileHeadInfo head;
	int size;
	CodeInfo code[0];
	ShareCodeReply() {
		head.type = FileInfoType::SHARE_REPLY;
		head.len = sizeof(ShareCodeReply);
	}
};
struct ShareCodeDownLoad
{
	FileHeadInfo head;
	int fileId;
	int userId;
	char code[5];
	ShareCodeDownLoad() {
		head.type = FileInfoType::SHARE_DOWNLOAD_FILE;
		head.len = sizeof(ShareCodeDownLoad);
		memset(code, 0, 5);
	}
};

struct ShareCodeDownLoadReply
{
	FileHeadInfo head;
	char filePath[128];
	ShareCodeDownLoadReply() {
		head.type = FileInfoType::SHARE_DOWNLOAD_REPLY;
		head.len = sizeof(ShareCodeDownLoadReply);
	}
};


#define HEAD_LEN 8
#define PRINT_ERROR(x) cout <<"[  "<< (x)<<":"<<WSAGetLastError() <<" "<<__FUNCTION__<<" "<<__LINE__<<"  ]"<<endl
#define PRINT_NORMAL(x) cout <<"[              " << (x) <<"                    ]"<<endl
#define MAX_LINKER 200
