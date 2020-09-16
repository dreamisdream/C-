#include "stdafx.h"
#include "MySQLInterface.h"

mutex  g_m;

void MySQLInterface::init()
{
	ErrorNum = 0;
	ErrorInfo = "ok";
	mysql_library_init(0, NULL, NULL);
	mysql_init(&MysqlInstance);
	mysql_options(&MysqlInstance, MYSQL_SET_CHARSET_NAME, "gbk");
}

MySQLInterface::~MySQLInterface()
{

}

MySQLInterface* MySQLInterface::getInstance()
{
	if (m_object != nullptr)
	{
		return m_object;
	}
	g_m.lock();
	if (m_object == nullptr)
	{
		m_object = new MySQLInterface;
		cout << "m_object = new MySQLInterface;" << endl;
	}
	g_m.unlock();
	return m_object;
}


// ����������Ϣ
void MySQLInterface::SetMySQLConInfo(char* server, char* username, char* password, char* database, int port)
{
	init();
	MysqlConInfo.server = server;
	MysqlConInfo.user = username;
	MysqlConInfo.password = password;
	MysqlConInfo.database = database;
	MysqlConInfo.port = port;
}

// ������
bool MySQLInterface::Open()
{
	if (mysql_real_connect(&MysqlInstance, MysqlConInfo.server, MysqlConInfo.user,
		MysqlConInfo.password, MysqlConInfo.database, MysqlConInfo.port, 0, 0) != NULL)
	{
		return true;
	}
	else
	{
		ErrorIntoMySQL();
		return false;
	}
}

// �Ͽ�����
void MySQLInterface::Close()
{
	mysql_close(&MysqlInstance);
}
MySQLInterface* MySQLInterface::m_object = nullptr;
//��ȡ����
bool MySQLInterface::Select(const std::string& Querystr, std::vector<std::vector<std::string> >& data)
{
	if (0 != mysql_query(&MysqlInstance, Querystr.c_str()))
	{
		ErrorIntoMySQL();
		return false;
	}

	Result = mysql_store_result(&MysqlInstance);

	// ������
	int row = mysql_num_rows(Result);
	int field = mysql_num_fields(Result);

	MYSQL_ROW line = NULL;
	line = mysql_fetch_row(Result);

	int j = 0;
	std::string temp;
	std::vector<std::vector<std::string> >().swap(data);
	while (NULL != line)
	{
		std::vector<std::string> linedata;
		for (int i = 0; i < field; i++)
		{
			if (line[i])
			{
				temp = line[i];
				linedata.push_back(temp);
			}
			else
			{
				temp = "";
				linedata.push_back(temp);
			}
		}
		line = mysql_fetch_row(Result);
		data.push_back(linedata);
	}
	return true;
}

// ��������
bool MySQLInterface::Query(const std::string& Querystr)
{
	if (mysql_query(&MysqlInstance, Querystr.c_str()) == 0)
	{
		return true;
	}
	ErrorIntoMySQL();
	return false;
}

// ���벢��ȡ�����ID,����Զ�����ID
int MySQLInterface::GetInsertID(const std::string& Querystr)
{
	if (!Query(Querystr))
	{
		ErrorIntoMySQL();
		return ERROR_QUERY_FAIL;
	}
	// ��ȡID
	return mysql_insert_id(&MysqlInstance);
}

//������Ϣ
void MySQLInterface::ErrorIntoMySQL()
{
	ErrorNum = mysql_errno(&MysqlInstance);
	ErrorInfo = mysql_error(&MysqlInstance);
}