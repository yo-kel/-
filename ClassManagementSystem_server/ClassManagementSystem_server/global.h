#pragma once

#include <stdio.h>
#include <winsock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <chrono>
#include <thread>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <string>
#include <sstream>


#pragma comment(lib,"Ws2_32.lib")
#define LL long long

using namespace std::chrono;

extern std::string classroomName;//应该从配置文件中读取

#define ServerPort 6666
#define ClientMax 105

struct ClientInfo {
	milliseconds loginTime;
	int authentication;
	std::string sid;
	std::string classroom;
	std::string subject;
	std::string position;
};

struct _client {
	bool con;
	milliseconds conTime;
	sockaddr_in addr;
	SOCKET sock;
	fd_set set;
	int iResult;
	ClientInfo* clientInfo;
};

extern SOCKET serverSocket;
extern _client* client[ClientMax];