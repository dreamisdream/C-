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
		cout << "������;" << endl;
		break;
	case FileInfoType::SHARE_INFOS:
		getAllShareFileInfo();
		break;
	case FileInfoType::SHARE:
		cout << "���յ��ͻ��˵�����      �����ļ�����" << endl;
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

//�û���¼�߼�����
void DataManager::login(UserInfo &info)
{
	cout << info.type << "--" << info.userName << "__" << info.password << endl;
	UserInfoReply reply;
	sqlDeal->selectUserInfo(info,reply);
	cout << reply.type << "--" << reply.len << "--" << reply.success << endl;
	int ret = send(client, (char*)&reply, reply.len, 0);
	if (ret <= 0)
	{
		PRINT_ERROR("�쳣");
		closesocket(client);
		return;
	}
	else
	{
		// ��¼�ɹ� ʵ������־�ಢ�����ļ� 
		cout << "��־�ɹ�����" << endl;
		string str = info.userName;
		str.append("����½�ɹ�");
		/*if (reply.success)
		{
			log = new Logging(reply.userId);
			log->info(str);
		}*/
	}
}
//�û�ע���߼�����
void  DataManager::registerUser(RegisterInfo &info)
{
	registerInfoReply reply;
	sqlDeal->selectRegisterUserInfo(info , reply);
	int temp = send(client, (char*)&reply, reply.len, 0);
	cout << reply.type << "--" << reply.len << "--" << reply.success << endl;
	createUserPath(info.userName);//haoyutong

	if (temp <= 0)
	{
		PRINT_ERROR("�쳣");
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
	cout << "�½��û�Ŀ¼" << endl;
	path p(newpath);
	create_directory(p);

}
// �ϴ��ļ�����ҵ��
void DataManager::upFileReq(UpFileRequest &info)
{
	//cout << "�ļ��ϴ�����:" << info.fileName << "--" << info.fileId <<"  filesize:"<<info.fileSize<< endl;
	if (fileTransfrom == nullptr)
	{
		fileTransfrom = new FileTranfrom(info.fileName, true);
	}
	sqlDeal->addOneFileToSql(info,"t_upload");
	UpFileRequestReply reply;
	reply.fileId = info.fileId;
	reply.userId = info.userId;
	cout << "�ϴ������е�userId��" << reply.userId << endl;
	cout << "���ϴ�����Id��" << reply.fileId << endl;
	strcpy_s(reply.filePath, info.filePath);
	reply.port = fileTransfrom->getPort();
	int ret = send(client, (char*)&reply, reply.len, 0);

	cout << "ret::" << ret << "   port:" << reply.port << endl;
	
	return;

	if (selectExist(info.filePath,"t_files") ) {
		// �ļ��Ѵ���
		if (!selectExist(info.filePath, "t_upload")) {
			addOneFile(info.filePath, "t_upload");
		}
		UpFileRequestReply reply;
		int len;
		send(client, (char*)&reply, len, 0);
	}
	else
	{
		// �������ݿ��¼
		addOneFile(info.filePath,"t_files");
		// �����µ�socket
		FileTranfrom *fileTransfrom = new FileTranfrom("filename",info.userId, true);
		fileTransfrom->init();

		UpFileRequestReply reply;
		strcpy_s(reply.filePath , info.filePath);
		reply.port = fileTransfrom->getPort();
		send(client, (char*)&reply, reply.len, 0);
	}
}
// �����ļ�����ҵ��
void DataManager::downFileReq(DownFileRequest &info)
{
	cout << ":::::::�յ������ļ�����" << endl;
	if (fileTransfrom == nullptr)
	{
		fileTransfrom = new FileTranfrom(info.fileName,info.userId, true);
	}

	DownFileRequestReply reply = sqlDeal->downloadFileInfo(info.fileId);
	reply.fileId = info.fileId;
	reply.userId = info.userId;
	cout << "��������ظ��е�userId��" << reply.userId << endl;
	reply.port = fileTransfrom->getPort();
	cout << "�˿ںţ�" << reply.port << endl;
	cout << "socket-client:" << client<<endl;

	int ret = send(client, (char*)&reply, reply.len, 0);
	if (ret == -1)
	{
		cout << "����ʧ��--"<<"�����룺" << WSAGetLastError() << endl;
	}
	cout << "�ظ��˿ͻ��ˣ�" << ret << "���ֽ�" << endl;

	std::cout << "ret::" << ret << "   port:" << reply.port << endl;
	return;

	
// 	if (selectExist(info.fileId)) {
// 		 //�ļ��Ѵ���
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
		* �����ļ�ҵ��
		* ���ݿ��ѯ MysqlPacked::sqlDeal ��������  ����Ҫ�Լ��޸�
		1.��ѯ�ļ��Ƿ���� ���ڻظ��ļ��Ѵ��� ��һ���µ�socket ���ض˿ں�
	*/

	//DownFileReply reply;
	//bool ret =sqlDeal->selectFileExist(info.filePath, info.head.userId);
	//if (!ret) 
	//{
	//	// �ļ�������
	//	reply.success = 0;
	//	send(client, (char*)&reply, reply.head.len, 0);
	//	return ;
	//}

	//reply = sqlDeal->selectFile(info);
	//ret = send(client, (char*)&reply, reply.head.len, 0);
	//if (ret != reply.head.len)
		cout << "����ʧ��" << endl;
}

void  DataManager::upFile(UpFile &info)
{
	/*
		* �ϴ��ļ�ҵ��
		* ���ݿ��ѯ MysqlPacked::sqlDeal ��������  ����Ҫ�Լ��޸�
	*/
	//cout << info.filePath << "---" << info.fileName << endl;
	//UpFileReply reply;
	//bool ret = sqlDeal->selectFileExist(info.filePath, info.head.userId);
	//if (!ret)
	//{
	//	// �ļ�������
	//	reply.success = 0;
	//	send(client, (char*)&reply, reply.head.len, 0);
	//	return;
	//}
	//reply = sqlDeal->insertFile(info);
	// ret = send(client, (char*)&reply, reply.head.len, 0);
	//if (ret != reply.head.len)
		cout << "����ʧ��" << endl;
}

//dong
void  DataManager::selectPath(SelectFiles &info)
{
	/*
		* ѡ��ҳ������ҵ���߼�
		* ���ݿ��ѯ MysqlPacked::sqlDeal ��������  ����Ҫ�Լ��޸�
	*/
	cout << "��ѯ���ݿ�ǰ" << endl;
	SelectFilesReply* filesReply = sqlDeal->selectFromFilePath(info);
	filesReply->file_status = 1;
	bool result = filesReply->sucess;
	if (result)
	{
		int send_len = send(client, (char*)filesReply, filesReply->head.len, 0);
		if (send_len <= 0)
		{
			//log->error("�ļ��б�������Ӧʧ��");
			cout << send_len << "�ļ��б�������Ӧʧ��";
		}
		else
		{
			//log->info("�����ļ��б�");
			cout << send_len << "�����ļ��б�";
		}
		delete filesReply;
		filesReply = nullptr;
	}
	else
	{
		//log->error("��ѯ�б��ָ��");
		cout << "��ָ��" << endl;
	}
}


//dong
void DataManager::deleteFile(DeleteFileStruct &info)
{
	/*
	* ɾ���ļ�ҵ���߼�
	* ���ݿ��ѯ MysqlPacked::sqlDeal ��������  ����Ҫ�Լ��޸�
	*/
	bool result = sqlDeal->deleteFile(info.fileId, info.userId);
	DeleteFileReply fileReply;
	fileReply.fileId = info.fileId;
	if (result)
	{
		cout << "�ļ�ɾ���ɹ�" << endl;
		//log->info("�ļ�ɾ���ɹ�");
		fileReply.isDelete = 1;
	}
	else
	{
		cout << "�ļ�ɾ��ʧ��" << endl;
		//log->info("�ļ�ɾ��ʧ��");
		fileReply.isDelete = 0;
	}
	int send_len = send(client, (char*)&fileReply, fileReply.head.len, 0);
	if (send_len > 0)
	{
		cout << "�ļ��б�ɾ���ļ���Ϣ���ؿͻ��˳ɹ�" << endl;
	}
	else
	{
		cout << "�ļ��б�ɾ���ļ���Ϣ���ؿͻ���ʧ��" << endl;
	}
}

//����վɾ���ļ�
void DataManager::recycleDeleteFile(RecycleDeleteFileStruct &info)
{
	cout << "�ͻ��˻���վ" << endl;
	cout << info.fileId << endl;

	bool result = sqlDeal->recycleDeleteFile(info.fileId);
	RecycleDeleteFileReply recycleReply;
	recycleReply.fileId = info.fileId;
	if (result)
	{
		cout << "ɾ������վ�������ݿ�״̬�ɹ�" << endl;
		recycleReply.succeed = 1;
	}
	else
	{
		cout << "ɾ������վ�������ݿ�״̬ʧ��" << endl;
		recycleReply.succeed = 0;
	}


	int send_len = send(client, (char*)&recycleReply, recycleReply.head.len, 0);
	if (send_len > 0)
	{
		cout << "����վɾ���ļ���Ϣ���ؿͻ��˳ɹ�" << endl;
	}
	else
	{
		cout << "����վɾ���ļ���Ϣ���ؿͻ���ʧ��" << endl;
	}
}
//����վ�ظ�����  hao
void DataManager::recycleRecoverFile(RecyRecoverDataStruct &info)
{
	bool result = sqlDeal->cycleRecycleFile(info.fileid);

	RecycleRecoverFileReply recoverReply;
	recoverReply.fileId = info.fileid;
	if (result)
	{
		cout << "����վ�ָ����ݿ�״̬�ɹ�------------------------------------" << endl;
		recoverReply.succeed = 1;
	}
	else
	{
		cout << "����վ�ָ����ݿ�״̬�ɹ�ʧ��" << endl;
		recoverReply.succeed = 0;
	}

	int send_len = send(client, (char*)&recoverReply, recoverReply.head.len, 0);
	if (send_len > 0)
	{
		cout << "����վ�ָ��ļ���Ϣ���ؿͻ��˳ɹ�" << endl;
	}
	else
	{
		cout << "����վ�ָ��ļ���Ϣ���ؿͻ���ʧ��" << endl;
	}
}

//�ͻ����������վ����
void DataManager::recycleRequestData(RequestRecyleDataStruct &info)
{
	SelectFilesReply* filesReply = sqlDeal->recycleRequestData(info.head.userId);
	bool result = filesReply->sucess;
	if (result)
	{
		int send_len = send(client, (char*)filesReply, filesReply->head.len, 0);
		if (send_len <= 0)
		{
			//log->error("����վ������Ӧʧ��");
			cout << send_len << "����վ������Ӧʧ��";
		}
		else
		{
			//log->info("���ػ���վ");
			cout << send_len << "���ػ���վ"<<endl;
		}
		delete filesReply;
		filesReply = nullptr;
	}
	else
	{
		//log->error("��ѯ����վ��ָ��");
		cout << "��ѯ����վ��ָ��" << endl;
	}
}

void DataManager::requestLogWidUserId(LogUserId &info)
{
	LogUserId *reply = sqlDeal->requestLogWidUserId();
	if (reply->success)
	{
		cout << "��־���ܽӿڻ�ȡ�û�ID �ɹ�" << endl;
		int len_send = send(client,(char*)reply,reply->len,0);
		if (len_send > 0)
		{
			cout << "��־���ܽӿڻ�ȡ�û�ID ���ؿͻ��˳ɹ�" << endl;
		}
		else
		{
			cout << "��־���ܽӿڻ�ȡ�û�ID ���ؿͻ���ʧ��" << endl;
		}
		delete reply;
		reply = nullptr;
	}
	else
	{
		cout << "��־���ܽӿڻ�ȡ�û�ID ʧ��" << endl;
	}
}

//�ͻ��������½�Ŀ¼
void DataManager::requestCreatePath(CreatePathStruct &info)
{
	/*
		1���½�Ŀ¼
		2���������ݿ�     
	*/
	string path = info.path;
	string newPath = info.newPath;
	newPath = path + "/" + newPath;
	createPath(newPath);

	bool result= sqlDeal->createPath(info);

	if (result)
	{
		cout << "�½�Ŀ¼��Ϣ�������ݿ�ɹ�" << endl;
		UpDateFilelist data;
		send(client, (char*)&data, data.len, 0);
	}
	else
	{
		cout << "�½�Ŀ¼��Ϣ�������ݿ�ʧ��" << endl;
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


//dong ��ѯ��־�ӿ�
void DataManager::selectLog(LogRequestStruct &info)
{
	LogStruct* logReply = sqlDeal->logRequestData(info);
	if(logReply->success)
	{
		int len_send = send(client,(char*)logReply,logReply->len,0);
		if (len_send < 1)
		{
			cout << "��־�ӿڷ��ظ��ͻ�������ʧ��" << endl;
		}
		else
		{
			cout << "len_send = " << len_send << endl;
			cout << "��־�ӿڷ��ؿͻ������ݳɹ�" << endl;
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
		PRINT_ERROR("������Ϣ�Ļظ�");
	}
}

void DataManager::upShareInfo(ShareCodeReq &info)
{
	sqlDeal->saveShareCode(info);
}

void DataManager::ShareInfoGetDownLoad(ShareCodeDownLoad &info)
{
	// ��ѯsql	���ؽ����
}
