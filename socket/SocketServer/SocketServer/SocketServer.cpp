// SocketServer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Server.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;
int main()
{
	Server s;
	s.run();
	//cout << "输入r开启客户端：" << endl;
	//string cmd;
	//while (cin>>cmd) {
	//	if (cmd == "r")
	//		s.run();
	//	else if(cmd == "e")
	//		break;
	//}
    return 0;
}

