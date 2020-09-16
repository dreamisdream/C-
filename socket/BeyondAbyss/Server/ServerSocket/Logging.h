#pragma once

#include<iostream>
#include<string>
#include<sstream>
#include<fstream>
#include<time.h>  
#include<filesystem>

#include "MySQLInterface.h" 

using namespace std;
using namespace std::tr2::sys;

class Logging
{
private:
	ofstream of;
	ostringstream oss;
	ostringstream sql;
	MySQLInterface* mysql_db;
public:
	Logging(int userID);
	~Logging();
public:
	void info(const string& info);
	void debug( const string& info);
	void warning(const string& info);
	void error( const string& info);
	void fatal( const string& info);
private:
	int userID;
	void openDataBase();
	void checkPath();
	string getTime();
	void write(const string& info);
	bool saveMysql(const string&currentTime, const string& info, int level);
};