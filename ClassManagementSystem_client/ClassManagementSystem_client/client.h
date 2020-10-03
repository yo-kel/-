#pragma once
#include <stdio.h>
#include <winsock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <iostream>
#include <chrono>

#pragma comment(lib,"Ws2_32.lib")
using namespace std::chrono;


#define ClientPort 6666
#define BufferMax 500
#define ServerAddress "127.0.0.1"//考虑到实际使用，应可以修改

char* buffer[BufferMax];

SOCKET clientSocket;

struct ServerInfo {
	milliseconds conTime;
};

struct _server {
	bool con;
	sockaddr_in addr;
	SOCKET sock;
	fd_set set;
	int iResult;
	ServerInfo serverInfo;
};