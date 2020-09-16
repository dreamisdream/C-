#include "stdafx.h"
#include "Logging.h"


Logging::Logging(int userID)
{	
	this->userID = userID;
	openDataBase();
	checkPath();
	string filename = "log/loginfo.log";
	of = ofstream(filename, ios::app);
}

Logging::~Logging()
{
	of.close();
	mysql_db->Close();
}

void Logging::openDataBase()
{
	mysql_db = MySQLInterface::getInstance();
	mysql_db->SetMySQLConInfo("127.0.0.1", "root", "root", "beyondabyss", 3306);
	if (!mysql_db->Open())
	{
		cout << mysql_db->ErrorNum << " : " << mysql_db->ErrorInfo << endl;
	}
}

void Logging::info(const string& info)
{
	string currentTime = getTime();
	int level = 1;
	oss << currentTime << "  (user£º"<<userID<<")  [INFO] " << info << endl;
	this->write(oss.str());
	this->saveMysql(currentTime,info,level);
}
void Logging::debug( const string& info)
{
	string currentTime = getTime();
	int level = 2;
	oss << getTime() << "  (user£º" << userID << ")  [DEBUG] " << info << endl;
	this->write(oss.str());
	this->saveMysql(currentTime, info, level);
}

void Logging::warning(const string& info)
{
	string currentTime = getTime();
	int level = 3;
	oss << getTime() << "  (user£º" << userID << ")  [WARNING] " << info << endl;
	this->write(oss.str());
	this->saveMysql(currentTime, info, level);
}
void Logging::error(const string& info)
{
	string currentTime = getTime();
	int level = 4;
	oss << getTime() << "  (user£º" << userID << ")  [ERROR] " << info << endl;
	this->write(oss.str());
	this->saveMysql(currentTime, info, level);
}

void Logging::fatal( const string& info)
{
	string currentTime = getTime();
	int level = 5;
	oss << getTime() << "  (user£º" << userID << ")  [FARAL] " << info << endl;
	this->write(oss.str());
	this->saveMysql(currentTime, info, level);
}

void Logging::checkPath()
{
	string newPath = "log/";
	path p(newPath);
	create_directory(p);
}

string Logging::getTime()
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

void Logging::write(const string& info)
{
	cout << info << endl;
	of << info;
	oss.str("");
}

bool Logging::saveMysql(const string&currentTime, const string& info, int level)
{
	//insert into t1_student(id, name, sex, age) values(1, 'Àî¹ú¶°', 'ÄÐ', 28);
	sql << "insert into T_log(user_id,log_time,level,info) values("
		<< userID << ","
		<< "'" << currentTime << "'" << ","
		<< level << ","
		<< "'" << info << "')";
	cout << sql.str() << endl;
	bool result = mysql_db->Query(sql.str());
	sql.str("");
	return result;
}