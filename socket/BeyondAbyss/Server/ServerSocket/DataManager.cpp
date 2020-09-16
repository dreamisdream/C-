#include "stdafx.h"

#include "MysqlPacked.h"
#include "DataManager.h"
#include "data.h"
#include <assert.h>
#include <time.h>
#include <sstream>

using namespace std;
FileTranfrom *DataManager::fileTransfrom = nullptr;
DataManager::DataManager(SOCKET client)
{
	sqlDeal = new MysqlPacked;
	this->client = client;
}

DataManager::DataManager(int type, char * buf, int len)
{

}

DataManager::~DataManager()
{
	delete sqlDeal;
	sqlDeal = nullptr;
}

void DataManager::addData(int type, char * buf)
{
	cout << "type:" << type;
	switch (type)
	{
	case FileInfoType::LOGIN:
		login(*(UserInfo*)buf);
		break;
	case FileInfoType::REGISTER:
		registerUser(*(RegisterInfo*)buf);
		break;
	case FileInfoType::UP_FILE_REQUEST:
		upFileReq(*(UpFileRequest*)buf);
		break;
	case FileInfoType::DOWN_FILE_REQUEST:
		downFileReq(*(DownFileRequest*)buf);
		break;
	case FileInfoType::SELECT_FILES:
		selectPath(*(SelectFiles*)buf);
		break;
	case FileInfoType::LOG_REQUEST://dong
		selectLog(*(LogRequestStruct*)buf);
		break;
	case FileInfoType::DELETE_FILE:
		deleteFile(*(DeleteFileStruct*)buf);
		break;
	case FileInfoType::RECYCLE_DELETE_REQUEST:
		recycleDeleteFile(*(RecycleDeleteFileStruct*)buf);
		break;
	case FileInfoType::RECYCLE_REQUEST_DATA:
		recycleRequestData(*(RequestRecyleDataStruct*)buf);
		break;
	case FileInfoType::RECYCLE_RECOVER_REQUEST:
		recycleRecoverFile(*(RecyRecoverDataStruct*)buf);
		break;
	case FileInfoType::LOG_ALLUSER_ID:
		requestLogWidUserId(*(LogUserId*)buf);
		break;
	case FileInfoType::CRETE_NEW_PATH://
		requestCreatePath(*(CreatePathStruct*)buf);
		break;
	case FileInfoType::HEART_PACK:
		cout << "心跳包;" << endl;
		break;
	case FileInfoType::SHARE_INFOS:
		getAllShareFileInfo();
		break;
	case FileInfoType::SHARE:
		cout << "接收到客户端的数据      分享文件保存" << endl;
		//void upShareInfo(ShareCodeReq &);
		upShareInfo(*(ShareCodeReq*)buf);
		break;
	case FileInfoType::SHARE_DOWNLOAD_FILE:
		ShareInfoGetDownLoad(*(ShareCodeDownLoad*)buf);
		break;
	default:
		break;
	}
	return ;
}

//用户登录逻辑梳理
void DataManager::login(UserInfo &info)
{
	cout << info.type << "--" << info.userName << "__" << info.password << endl;
	UserInfoReply reply;
	sqlDeal->selectUserInfo(info,reply);
	cout << reply.type << "--" << reply.len << "--" << reply.success << endl;
	int ret = send(client, (char*)&reply, reply.len, 0);
	if (ret <= 0)
	{
		PRINT_ERROR("异常");
		closesocket(client);
		return;
	}
	else
	{
		// 登录成功 实例化日志类并保存文件 
		cout << "日志成功保存" << endl;
		string str = info.userName;
		str.append("：登陆成功");
		/*if (reply.success)
		{
			log = new Logging(reply.userId);
			log->info(str);
		}*/
	}
}
//用户注册逻辑梳理
void  DataManager::registerUser(RegisterInfo &info)
{
	registerInfoReply reply;
	sqlDeal->selectRegisterUserInfo(info , reply);
	int temp = send(client, (char*)&reply, reply.len, 0);
	cout << reply.type << "--" << reply.len << "--" << reply.success << endl;
	createUserPath(info.userName);//haoyutong

	if (temp <= 0)
	{
		PRINT_ERROR("异常");
		closesocket(client);
		return;
	}	
}

void DataManager::createUserPath(string path)
{
	CreatePathStruct data;
	strcpy_s(data.path,sizeof("userContents"), "userContents");
	strcpy_s(data.newPath, 256, path.c_str());
	data.userId = sqlDeal->getUserId(path);

	sqlDeal->createPath(data);
	string newPath = "userContents";
	checkPath(newPath);
	newPath = newPath + "/" + path;
	checkPath(newPath);
}



void DataManager::checkPath(string newpath)
{
	cout << "新建用户目录" << endl;
	path p(newpath);
	create_directory(p);

}
// 上传文件请求业务
void DataManager::upFileReq(UpFileRequest &info)
{
	//cout << "文件上传请求:" << info.fileName << "--" << info.fileId <<"  filesize:"<<info.fileSize<< endl;
	if (fileTransfrom == nullptr)
	{
		fileTransfrom = new FileTranfrom(info.fileName, true);
	}
	sqlDeal->addOneFileToSql(info,"t_upload");
	UpFileRequestReply reply;
	reply.fileId = info.fileId;
	reply.userId = info.userId;
	cout << "上传请求中的userId：" << reply.userId << endl;
	cout << "该上传任务Id：" << reply.fileId << endl;
	strcpy_s(reply.filePath, info.filePath);
	reply.port = fileTransfrom->getPort();
	int ret = send(client, (char*)&reply, reply.len, 0);

	cout << "ret::" << ret << "   port:" << reply.port << endl;
	
	return;

	if (selectExist(info.filePath,"t_files") ) {
		// 文件已存在
		if (!selectExist(info.filePath, "t_upload")) {
			addOneFile(info.filePath, "t_upload");
		}
		UpFileRequestReply reply;
		int len;
		send(client, (char*)&reply, len, 0);
	}
	else
	{
		// 创建数据库纪录
		addOneFile(info.filePath,"t_files");
		// 创建新的socket
		FileTranfrom *fileTransfrom = new FileTranfrom("filename",info.userId, true);
		fileTransfrom->init();

		UpFileRequestReply reply;
		strcpy_s(reply.filePath , info.filePath);
		reply.port = fileTransfrom->getPort();
		send(client, (char*)&reply, reply.len, 0);
	}
}
// 下载文件请求业务
void DataManager::downFileReq(DownFileRequest &info)
{
	cout << ":::::::收到下载文件请求" << endl;
	if (fileTransfrom == nullptr)
	{
		fileTransfrom = new FileTranfrom(info.fileName,info.userId, true);
	}

	DownFileRequestReply reply = sqlDeal->downloadFileInfo(info.fileId);
	reply.fileId = info.fileId;
	reply.userId = info.userId;
	cout << "下载请求回复中的userId：" << reply.userId << endl;
	reply.port = fileTransfrom->getPort();
	cout << "端口号：" << reply.port << endl;
	cout << "socket-client:" << client<<endl;

	int ret = send(client, (char*)&reply, reply.len, 0);
	if (ret == -1)
	{
		cout << "发送失败--"<<"错误码：" << WSAGetLastError() << endl;
	}
	cout << "回复了客户端：" << ret << "个字节" << endl;

	std::cout << "ret::" << ret << "   port:" << reply.port << endl;
	return;

	
// 	if (selectExist(info.fileId)) {
// 		 //文件已存在
// 		FileTranfrom *fileTransfrom = new FileTranfrom("filename", false);
		
		

// 		DownFileRequestReply reply;
// 		int len;
// 		send(client, (char*)&reply, len, 0);
// 	}
// 	else
// 	{
// 		DownFileRequestReply reply;
// 		int len;
// 		send(client, (char*)&reply, len, 0);
// 	}

}


void  DataManager::selectFile(DownFile &info)
{
	/*
		* 下载文件业务
		* 数据库查询 MysqlPacked::sqlDeal 方法调用  有需要自己修改
		1.查询文件是否存在 存在回复文件已存在 起一个新的socket 返回端口号
	*/

	//DownFileReply reply;
	//bool ret =sqlDeal->selectFileExist(info.filePath, info.head.userId);
	//if (!ret) 
	//{
	//	// 文件不存在
	//	reply.success = 0;
	//	send(client, (char*)&reply, reply.head.len, 0);
	//	return ;
	//}

	//reply = sqlDeal->selectFile(info);
	//ret = send(client, (char*)&reply, reply.head.len, 0);
	//if (ret != reply.head.len)
		cout << "发送失败" << endl;
}

void  DataManager::upFile(UpFile &info)
{
	/*
		* 上传文件业务
		* 数据库查询 MysqlPacked::sqlDeal 方法调用  有需要自己修改
	*/
	//cout << info.filePath << "---" << info.fileName << endl;
	//UpFileReply reply;
	//bool ret = sqlDeal->selectFileExist(info.filePath, info.head.userId);
	//if (!ret)
	//{
	//	// 文件不存在
	//	reply.success = 0;
	//	send(client, (char*)&reply, reply.head.len, 0);
	//	return;
	//}
	//reply = sqlDeal->insertFile(info);
	// ret = send(client, (char*)&reply, reply.head.len, 0);
	//if (ret != reply.head.len)
		cout << "发送失败" << endl;
}

//dong
void  DataManager::selectPath(SelectFiles &info)
{
	/*
		* 选择页面内容业务逻辑
		* 数据库查询 MysqlPacked::sqlDeal 方法调用  有需要自己修改
	*/
	cout << "查询数据库前" << endl;
	SelectFilesReply* filesReply = sqlDeal->selectFromFilePath(info);
	filesReply->file_status = 1;
	bool result = filesReply->sucess;
	if (result)
	{
		int send_len = send(client, (char*)filesReply, filesReply->head.len, 0);
		if (send_len <= 0)
		{
			//log->error("文件列表数据响应失败");
			cout << send_len << "文件列表数据响应失败";
		}
		else
		{
			//log->info("返回文件列表");
			cout << send_len << "返回文件列表";
		}
		delete filesReply;
		filesReply = nullptr;
	}
	else
	{
		//log->error("查询列表空指针");
		cout << "空指针" << endl;
	}
}


//dong
void DataManager::deleteFile(DeleteFileStruct &info)
{
	/*
	* 删除文件业务逻辑
	* 数据库查询 MysqlPacked::sqlDeal 方法调用  有需要自己修改
	*/
	bool result = sqlDeal->deleteFile(info.fileId, info.userId);
	DeleteFileReply fileReply;
	fileReply.fileId = info.fileId;
	if (result)
	{
		cout << "文件删除成功" << endl;
		//log->info("文件删除成功");
		fileReply.isDelete = 1;
	}
	else
	{
		cout << "文件删除失败" << endl;
		//log->info("文件删除失败");
		fileReply.isDelete = 0;
	}
	int send_len = send(client, (char*)&fileReply, fileReply.head.len, 0);
	if (send_len > 0)
	{
		cout << "文件列表删除文件信息返回客户端成功" << endl;
	}
	else
	{
		cout << "文件列表删除文件信息返回客户端失败" << endl;
	}
}

//回收站删除文件
void DataManager::recycleDeleteFile(RecycleDeleteFileStruct &info)
{
	cout << "客户端回收站" << endl;
	cout << info.fileId << endl;

	bool result = sqlDeal->recycleDeleteFile(info.fileId);
	RecycleDeleteFileReply recycleReply;
	recycleReply.fileId = info.fileId;
	if (result)
	{
		cout << "删除回收站功能数据库状态成功" << endl;
		recycleReply.succeed = 1;
	}
	else
	{
		cout << "删除回收站功能数据库状态失败" << endl;
		recycleReply.succeed = 0;
	}


	int send_len = send(client, (char*)&recycleReply, recycleReply.head.len, 0);
	if (send_len > 0)
	{
		cout << "回收站删除文件信息返回客户端成功" << endl;
	}
	else
	{
		cout << "回收站删除文件信息返回客户端失败" << endl;
	}
}
//回收站回复数据  hao
void DataManager::recycleRecoverFile(RecyRecoverDataStruct &info)
{
	bool result = sqlDeal->cycleRecycleFile(info.fileid);

	RecycleRecoverFileReply recoverReply;
	recoverReply.fileId = info.fileid;
	if (result)
	{
		cout << "回收站恢复数据库状态成功------------------------------------" << endl;
		recoverReply.succeed = 1;
	}
	else
	{
		cout << "回收站恢复数据库状态成功失败" << endl;
		recoverReply.succeed = 0;
	}

	int send_len = send(client, (char*)&recoverReply, recoverReply.head.len, 0);
	if (send_len > 0)
	{
		cout << "回收站恢复文件信息返回客户端成功" << endl;
	}
	else
	{
		cout << "回收站恢复文件信息返回客户端失败" << endl;
	}
}

//客户端请求回收站数据
void DataManager::recycleRequestData(RequestRecyleDataStruct &info)
{
	SelectFilesReply* filesReply = sqlDeal->recycleRequestData(info.head.userId);
	bool result = filesReply->sucess;
	if (result)
	{
		int send_len = send(client, (char*)filesReply, filesReply->head.len, 0);
		if (send_len <= 0)
		{
			//log->error("回收站数据响应失败");
			cout << send_len << "回收站数据响应失败";
		}
		else
		{
			//log->info("返回回收站");
			cout << send_len << "返回回收站"<<endl;
		}
		delete filesReply;
		filesReply = nullptr;
	}
	else
	{
		//log->error("查询回收站空指针");
		cout << "查询回收站空指针" << endl;
	}
}

void DataManager::requestLogWidUserId(LogUserId &info)
{
	LogUserId *reply = sqlDeal->requestLogWidUserId();
	if (reply->success)
	{
		cout << "日志功能接口获取用户ID 成功" << endl;
		int len_send = send(client,(char*)reply,reply->len,0);
		if (len_send > 0)
		{
			cout << "日志功能接口获取用户ID 返回客户端成功" << endl;
		}
		else
		{
			cout << "日志功能接口获取用户ID 返回客户端失败" << endl;
		}
		delete reply;
		reply = nullptr;
	}
	else
	{
		cout << "日志功能接口获取用户ID 失败" << endl;
	}
}

//客户端请求新建目录
void DataManager::requestCreatePath(CreatePathStruct &info)
{
	/*
		1：新建目录
		2：插入数据库     
	*/
	string path = info.path;
	string newPath = info.newPath;
	newPath = path + "/" + newPath;
	createPath(newPath);

	bool result= sqlDeal->createPath(info);

	if (result)
	{
		cout << "新建目录信息插入数据库成功" << endl;
		UpDateFilelist data;
		send(client, (char*)&data, data.len, 0);
	}
	else
	{
		cout << "新建目录信息插入数据库失败" << endl;
	}
}


void DataManager::createPath(string newPath)
{
	path p(newPath);
	create_directory(p);
}
	
bool DataManager::addOneFile(string filepath,string table)
{
	UpFileRequest up;
	return sqlDeal->addOneFileToSql(up,table);
}
bool DataManager::selectExist(string filepath,string table)
{
	return sqlDeal->selectFileExist(filepath,table);

}

string DataManager::getNowTime()
{
	time_t raw_time;
	struct tm  *info;
	char buffer[24];
	time(&raw_time);
	info = localtime(&raw_time);
	strftime(buffer, 24, "%Y%m%d%H%M%S", info);
	return buffer ;
}


//dong 查询日志接口
void DataManager::selectLog(LogRequestStruct &info)
{
	LogStruct* logReply = sqlDeal->logRequestData(info);
	if(logReply->success)
	{
		int len_send = send(client,(char*)logReply,logReply->len,0);
		if (len_send < 1)
		{
			cout << "日志接口返回给客户端数据失败" << endl;
		}
		else
		{
			cout << "len_send = " << len_send << endl;
			cout << "日志接口返回客户端数据成功" << endl;
		}
		delete logReply;
		logReply = nullptr;
	}
}
void DataManager::getAllShareFileInfo()
{
	ShareCodeReply *reply  = sqlDeal->getShareCodeInfo();
	reply->head.type = FileInfoType::SHARE_REPLY;
	int retSend = send(client, (char *)reply, reply->head.len, 0);
	if (retSend <= 0){
		PRINT_ERROR("分享信息的回复");
	}
}

void DataManager::upShareInfo(ShareCodeReq &info)
{
	sqlDeal->saveShareCode(info);
}

void DataManager::ShareInfoGetDownLoad(ShareCodeDownLoad &info)
{
	// 查询sql	返回结果；
}
