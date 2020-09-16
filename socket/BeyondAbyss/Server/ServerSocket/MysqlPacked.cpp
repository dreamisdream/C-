#include "stdafx.h"
#include "MysqlPacked.h"
#include <sys/stat.h>
#include <string>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;
//using namespace std::filesystem;

MysqlPacked::MysqlPacked()
{
	loadConfig();
}

MysqlPacked::~MysqlPacked()
{
	db->Close();
}

void MysqlPacked::selectUserInfo(const UserInfo& userinfo, UserInfoReply &infoRet)
{
	string str = "select * from t_user where username = \"";
	str += userinfo.userName;
	str += "\" and  password = \"";
	str += userinfo.password;
	str += "\"";
	cout << str << endl;


	infoRet.type = FileInfoType::LOGIN_REPLY;
	infoRet.len = sizeof(UserInfoReply);

	vector<vector<string>> ret;
	bool retBool = db->Select(str, ret);
	cout << boolalpha << retBool << endl;
	if (ret.size())
	{
		infoRet.success = 1;
		infoRet.userId = atoi(ret[0][0].c_str());
	}
	else
	{
		infoRet.success = 0;
	}
	return ;
}

int MysqlPacked::getUserId(string userName)
{
	vector<vector<string>>ret;
	ostringstream sql;
	sql << "select id from t_user where username = "
		<<"'"<< userName<<"'";
	bool result = db->Select(sql.str(), ret);
	if (result)
	{
		int id = stoi(ret[0][0]);
		return id;
	}
	return 0;
}

void MysqlPacked::selectRegisterUserInfo(RegisterInfo& registerinfo,registerInfoReply &registerReply)
{
	//查用户名
	ostringstream sql_userName;  
	sql_userName << "select * from t_user where userName = "
		<< "'"
		<< registerinfo.userName
		<< "'";
	cout << "sql_userName=" << sql_userName.str()<< endl;
	vector<vector<string>> ret;
	bool nameResult=db->Select(sql_userName.str(), ret);
	cout << "ret.size()=" << ret.size() << endl;
	if (!nameResult)
	{
		cout << "查询用户名失败" << endl;
	}
	else
	{
		if (ret.size() > 0)
		{
			registerReply.success = 0;
			cout << "该用户已存在" << endl;
			return;
		}
		else
		{
			ostringstream sql_user;
			sql_user << "insert into t_user (username, password, phonenumber, identitycode, registeremail) values"
				<< "('"
				<< registerinfo.userName
				<< "','"
				<< registerinfo.password
				<< "',"
				<< registerinfo.phoneNumber
				<< ","
				<< registerinfo.identityCode
				<< ",'"
				<< registerinfo.Email
				<< "')";

			cout << "sql=" << sql_user.str() << endl;
			bool result = db->Query(sql_user.str());
			if (result == true)
			{
				registerReply.success = 1;
				cout << " 用户信息已保存至数据库 " << endl;

			}
			else
			{
				registerReply.success = 0;
				cout << "用户信息保存至数据库失败" << endl;
			}
		}
	}	
	ret.clear();
	return ;
}



UpFileReply MysqlPacked::insertFile(UpFile& fileinfo)
{

#define SQL_INSERT_FILE(x1,x2,x3,x4,x5,x6,x7,x8)	\
	"insert into t_user(user_id,is_file,filename,file_type,create_time,last_update_time,size,file_status) \
		values((x1),(x2),(x3),(x4),(x5),(x6),(x7),(x8))"	
	bool status = db->Query(SQL_INSERT_FILE(fileinfo.head.userId,fileinfo.isFile,fileinfo.fileName,fileinfo.fileType,fileinfo.nowTime,fileinfo.nowTime,fileinfo.fileSize,0));
	UpFileReply reply;
	if (status == true) 
	{
		reply.success = status;
	}
	else
	{
		reply.success = !status;
	}
	return reply;
}

DownFileReply MysqlPacked::selectFile(DownFile& fileinfo)
{
#define  SQL_SELECT_FILE(x1,x2) "select file_path,isfile,file_status,  from t_file where id =(x1) and filepath = (x2)"
	vector<vector<string>> ret;
	DownFileReply reply;
	bool status = db->Select(SQL_SELECT_FILE(fileinfo.head.userId, fileinfo.filePath), ret);
	if (status == true)
	{
		reply.success = status;
	}
	else
	{
		reply.success = !status;
	}
	return reply;
}

//dong
SelectFilesReply* MysqlPacked::selectFromFilePath(SelectFiles &fileinfo)
{
	string path = fileinfo.path;
	int user_id = fileinfo.head.userId;
	ostringstream sql;
	sql << "select * from t_files where user_id = "
		<< fileinfo.head.userId
		<< " and  file_status = 1";

	//sql.str("");  
	/*sql << "select * from t_file where user_id = 1 and file_path = '??~{!A~}??~{(&~}'";*/
	//sql << "select * from t_files where user_id = 3 and file_status = 1";
	vector<vector<string>> ret;
	//?~{($~}?????~{("~}????
	cout << "sql = " << sql.str() << endl;
	SelectFilesReply reply;
	bool status = db->Select(sql.str(),ret);
	if (status == true) 
	{
		cout << "获取文件列表查询数据库成功" << endl;
		size_t len_vector = ret.size();
		int len_total = sizeof(SelectFilesReply) + len_vector * sizeof(FileInfoStruct);
		SelectFilesReply *data = (SelectFilesReply*)malloc(len_total);
		memset(data, 0, len_total);
		data->head.type = FileInfoType::SELECT_FILES_REPLY;
		data->sucess = status;
		data->len = len_vector;
		data->head.len = len_total;
		for (size_t i = 0; i < len_vector; i++)
		{
			data->file[i].fileId = stoi(ret[i][0]);
			data->file[i].isFile = stoi(ret[i][2]);
			data->file[i].fileType = stoi(ret[i][4]);
			data->file[i].fileSize = stoll(ret[i][8]);

			strcpy_s(data->file[i].fileName, 128, ret[i][3].c_str());
			strcpy_s(data->file[i].filePath, 128, ret[i][4].c_str());
			strcpy_s(data->file[i].nowTime, 32, ret[i][6].c_str());
		}
		sql.str("");
		ret.clear();
		return data;
	}
	else
	{
		cout << "获取文件列表查询数据库失败" << endl;
		reply.sucess = status;
	}
	sql.str("");
	ret.clear();
	return  &reply;
}
// bool selectFileExist(std::string filepath ,std::string table);
bool MysqlPacked::selectFileExist(std::string filepath ,string table)
{
	ostringstream oss;
	oss << "select id from "; 
	oss << table;
	oss << "where file_path = \"";
	oss << filepath;
	oss << "\"";
	vector<vector<string>> ret;
	db->Select(oss.str(), ret);
	return stoi(ret[0][0]) != 0;
}

bool MysqlPacked::addOneFileToSql(UpFileRequest & info,string table)
{ 
    // 	INSERT INTO `t_upload` (`file_size`, `file_path`, `upload_time`) VALUES('32154', '45465', '2020-06-05 17:29:42');
	ostringstream oss;
	oss << "insert into "
		<< table
		<< "(file_size ,file_path,upload_time,task_id,user_id) values("
		<< info.fileSize
		<< ", '"
		<< info.filePath
		<< "', str_to_date('"
		<< getNowTime()
		<< "','%Y%m%d%H%i%s'),"
		<< info.fileId
		<< ", "
		<< info.userId
		<<")";

	cout <<"写入下载表的sql："<< oss.str() << endl;
	
	bool isOk = db->Query(oss.str());
	if (!isOk)
	{
		cout << "数据库记录创建失败";
	}
	return isOk;
}

bool MysqlPacked::updataFileInfo(UpFileReply info,string table )
{
	ostringstream oss;
	oss << "update "
		<< table
		<< " set now_size = "
		<< info.recvBytes
		<< ", last_updata_time = '"
		<< getNowTime()
		<< "' "
		<< " where file_path in ('"
		<<info.filePath
		<<"')  and user_id = "
		<< info.userId;

	cout << "更新上传表的sql:"<<oss.str() << endl;
	
	bool isOk = db->Query(oss.str());
	if (!isOk)
	{
		cout << "更新文件指针失败" << endl;
	}
	return isOk;
}

void MysqlPacked::saveShareCode(ShareCodeReq &info)
{
	ostringstream oss;
	// insert into t_share(file_id ,share_code) values(1, 'sdwe')
	oss << "insert into t_share(file_id ,share_code) values(";
	oss << info.fileId;
	oss << ", '";
	oss << info.code;
	oss << "')";
	cout << oss.str() << endl;
	
	int ret = db->Query(oss.str());
	if (!ret)
		PRINT_ERROR("数据库插入分享码错误");
}

bool MysqlPacked::uploadCompleted(UpFileReply &info)
{
	ostringstream oss;
	oss << "insert into t_files(user_id,filename,file_path,create_time,size)"
		<< "select user_id,SUBSTRING_INDEX(file_path, '/', -1),"
		<< "file_path,last_updata_time,file_size from t_upload"
		<< " where file_size = now_size and file_path in ('"
		<< info.filePath
		<< "')";
	cout << oss.str() << endl;
	bool isOk = db->Query(oss.str());
	if (!isOk)
	{
		cout << "t_files插入记录失败" << endl;
	}
	else
	{
		cout << "t_files插入记录成功" << endl;
	}
	return isOk;
}

string MysqlPacked::getNowTime()
{
	time_t raw_time;
	struct tm  *info;
	char buffer[24];
	time(&raw_time);
	info = localtime(&raw_time);
	strftime(buffer, 24, "%Y%m%d%H%M%S", info);
	return buffer;
}

int MysqlPacked::getFileSize(std::string filePath)
{
	std::ifstream  read_file(filePath.c_str(), ios::binary);
	string s;
	int fileSize = 0;
	if (read_file.is_open())
	{
		read_file.seekg(0, ios_base::end);//????????????
		istream::pos_type file_size = read_file.tellg();//?????~{!@~}?~{!c~}???????????~{!h~}??->???????~{!h~}??
		read_file.seekg(0, ios_base::beg);//????????????????
		return fileSize;
	}
	return 0;
}

string MysqlPacked::getFileName(std::string filepath)
{
	if (filepath.empty())
		return "";
	int pos = filepath.find_last_of("/\\");
	return filepath.substr(pos + 1);
}


//dong
bool MysqlPacked::deleteFile(int fileId, int userId)
{
	ostringstream sql;
	sql << "update t_files set file_status = "
		<< 0 
		<<" where user_id = "
		<< userId
		<<" and id = "
		<< fileId;
	bool result  = db->Query(sql.str());
	cout << "sql = " << sql.str()<<endl;
	cout << "result = " << result << endl;
	sql.str("");
	return result;
}


//haoyutong
bool MysqlPacked::recycleDeleteFile(int fileId)
{
	ostringstream sql;
	sql << "update t_files set file_status = -1 where id = "
		<< fileId;
	
	return db->Query(sql.str());
}

bool MysqlPacked::cycleRecycleFile(int fileid)
{
	ostringstream sql;
	sql << "update t_files set file_status = 1 where id = "
		<< fileid;

	return db->Query(sql.str());
	return false;
}


//haoyutong
SelectFilesReply* MysqlPacked::recycleRequestData(int userId)
{
	ostringstream sql;
	
	sql << "select * from t_files where user_id = "
		<< userId
		<< " and file_status = 0";
	cout << "sql = " << sql.str() << endl;;
	vector<vector<string>> ret;
	SelectFilesReply reply;
	reply.file_status = 0;
	bool status = db->Select(sql.str(), ret);
	if (status == true)
	{
		cout << "获取回收站列表查询数据库成功" << endl;
		size_t len_vector = ret.size();
		int len_total = sizeof(SelectFilesReply) + len_vector * sizeof(FileInfoStruct);
		SelectFilesReply *data = (SelectFilesReply*)malloc(len_total);
		memset(data, 0, len_total);
		data->head.type = FileInfoType::SELECT_FILES_REPLY;
		data->sucess = status;
		data->len = len_vector;
		data->head.len = len_total;
		for (size_t i = 0; i < len_vector; i++)
		{
			data->file[i].fileId = stoi(ret[i][0]);
			data->file[i].isFile = stoi(ret[i][2]);
			data->file[i].fileType = stoi(ret[i][4]);
			data->file[i].fileSize = stoll(ret[i][8]);

			strcpy_s(data->file[i].fileName, 128, ret[i][3].c_str());
			strcpy_s(data->file[i].filePath, 128, ret[i][5].c_str());
			strcpy_s(data->file[i].deleteTime, 32, ret[i][11].c_str());
		}
		sql.str("");
		ret.clear();
		return data;
	}
	else
	{
		cout << "获取文件列表查询数据库失败" << endl;
		reply.sucess = status;
	}
	sql.str("");
	ret.clear();
	return  &reply;
}

FileType MysqlPacked::getFileType(std::string filePath)
{
	//???~{!@~}??????????
	return FileType::TEXT;
}

bool MysqlPacked::isDir(std::string filePath)
{
	return filePath.find_first_of("/\\") == -1;
}

bool MysqlPacked::loadConfig()
{
	TiXmlDocument doc;
	if (!doc.LoadFile("example.xml"))
	{
		cout << doc.ErrorDesc() << endl;
		return false;
	}
	TiXmlElement* root = doc.FirstChildElement();
	if (root == NULL)
	{
		cerr << "Failed to load file: No root element." << endl;
		doc.Clear();
		return false;
	}
	vector<string> str;
	for (TiXmlElement* elem = root->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement())
	{
		string elemName = elem->Value();
		str.push_back(elem->Attribute("server"));
		str.push_back(elem->Attribute("username"));
		str.push_back(elem->Attribute("password"));
		str.push_back(elem->Attribute("database"));
		str.push_back(elem->Attribute("port"));
	}
	db = MySQLInterface::getInstance();
	int port = stoi(str[4].c_str());
	db->SetMySQLConInfo((char*)str[0].c_str(), (char*)str[1].c_str(), (char*)str[2].c_str(), (char*)str[3].c_str(), port);
	if (!db->Open())
	{
		PRINT_ERROR("数据库打开失败");
		return false;
	}
	doc.Clear();
	return true;
}

//下载请求后回复文件信息
DownFileRequestReply MysqlPacked::downloadFileInfo(int fileId)
{
	DownFileRequestReply fileInfo;
	


	ostringstream sql;
	cout << "传入的fileId:" << fileId;
	sql << "call proc_DownloadFile(" << fileId << ")";

	
	vector<vector<string>> info;
	bool isOk = db->Select(sql.str(), info);
	if (!isOk)
	{
		cout << "查询失败" << endl;
	}
	else
	{
		if (info.size() == 0)
		{
			cout << "没有该条记录" << endl;
		}
		else
		{
			cout << "path:" << info[0][0]
				<< "size:" << info[0][1]
				<< "pointer:" << info[0][2]
				<< endl;
			strcpy_s(fileInfo.filePath, info[0][0].c_str());
			fileInfo.fileSize = (__int64)stoll(info[0][1]);
			if (info[0][2].empty())
			{
				fileInfo.filePointer = 0;
			}
			else
			{
				fileInfo.filePointer = (__int64)stoll(info[0][2]);
			}
		}	
	}
	cout << "path:" << fileInfo.filePath
		<< "size:" << fileInfo.fileSize
		<< "pointer:" << fileInfo.filePointer
		<< endl;
	return fileInfo;
}


//dong 查询日志数据
LogStruct * MysqlPacked::logRequestData(LogRequestStruct & info)
{

	string sql = getLogSql(info.selectUserId, info.logLevle,info.sortStatus);
	cout << "sql = " << sql << endl;
	LogStruct reply;
	vector<vector<string>> ret;
	bool result = db->Select(sql,ret);
	if (result)
	{	
		cout << "日志获取接口查询数据库成功" << endl;
		int count = ret.size();
		int len = sizeof(LogStruct) + ret.size() * sizeof(LogInfo);
		LogStruct *data = (LogStruct*)malloc(len);
		memset(data,0,len);
		data->success = 1;
		data->type = FileInfoType::LOG_ALLINFO;
		data->len = len;
		data->count = count;
		for (size_t i = 0; i < ret.size(); i++)
		{
			data->log[i].id = stoi(ret[i][0]);
			data->log[i].user_id = stoi(ret[i][1]);
			data->log[i].level = stoi(ret[i][3]);
			memcpy(data->log[i].log_time,ret[i][2].c_str(), sizeof(ret[i][2]).c_str()+1);
			memcpy(data->log[i].log_info,ret[i][4].c_str(), sizeof(ret[i][4]).c_str()+1);
		}
		ret.clear();
		sql.clear();
		return data;
	}
	else
	{
		cout << "日志获取接口查询数据库失败" << endl;
		reply.success = 0;
	}
	return &reply;
}


//dong
LogUserId* MysqlPacked::requestLogWidUserId()
{
	string sql = "select distinct user_id from t_log";
	vector<vector<string>> ret;
	LogUserId reply;
	bool result = db->Select(sql,ret);
	if (result)
	{
		int count = ret.size();
		int len = sizeof(LogUserId) + count * sizeof(int);
		LogUserId* data = (LogUserId*)malloc(len);
		data->type = FileInfoType::LOG_ALLUSER_ID;
		data->len = len;
		data->count = count;
		data->success = 1;
		for (size_t i = 0; i < count; i++)
		{
			data->allId[i] = stoi(ret[i][0]);
		}
		return data;
	}
	else
	{
		reply.success = 0;
	}
	return &reply;
}

string MysqlPacked::getLogSql(int userId, int level, int sortStatus)
{
	/*
		????????????????????sql????
	*/
	stringstream sql;
	stringstream userSql;
	string temp;
	map<int, string>sortMap;
	map<int, string>levelMap;

	if (userId == -1)
	{
		userSql<< "";
		temp = " where ";
	}
	else
	{
		userSql << "where user_id = " << userId << " ";
		temp = " and ";
	}

	levelMap[-1] = " ";
	levelMap[1] = temp + "level = 1 ";
	levelMap[2] = temp + "level = 2 ";
	levelMap[3] = temp + "level = 3 ";
	levelMap[4] = temp + "level = 4 ";
	levelMap[5] = temp + "level = 5 ";

	sortMap[0] = "order by log_time asc";
	sortMap[1] = "order by log_time desc";

	sql << "select * from t_log "
		<< userSql.str()
		<< levelMap[level]
		<< sortMap[sortStatus];
	return sql.str();
}

bool MysqlPacked::createPath(CreatePathStruct & info)
{
	ostringstream sql;
	//strcpy_s(info.path,sizeof("root"),"root");
	sql << "insert into t_files( user_id,is_file,filename,file_type,file_path,create_time,size,property,file_status) values ("
		<< info.userId << ","
		<< 0 << ","
		<< "'" << info.newPath << "',"
		<< 0 << ","
		<< "'" << info.path << "',"
		<< "'" << getTime() << "',"
		<< 0 << ","
		<< 1 << ","
		<< 1 << ")";
	return db->Query(sql.str());
}

string MysqlPacked::getTime()
{
	string strtime;
	char buff[20];
	time_t now = time(NULL);
	tm time;
	localtime_s(&time, &now);
	strftime(buff, sizeof(buff), "%Y-%m-%d %H:%M:%S", &time);
	strtime = buff;
	return strtime;
}