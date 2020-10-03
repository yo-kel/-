#pragma once
#include <stdio.h>
#include <winsock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <iostream>
#include <chrono>

#pragma comment(lib,"Ws2_32.lib")

#define ServerPort 6666
#define ClientMax 105

using namespace std::chrono;

struct ClientInfo {
	milliseconds conTime;
};

struct _client {
	bool con;
	sockaddr_in addr;
	SOCKET sock;
	fd_set set;
	int iResult;
	ClientInfo clientInfo;
};

SOCKET serverSocket;
_client* client[ClientMax];

void Disconnect(_client* client);
int Send(_client* client, char* buffer, int sz);
int Recv(_client* client, char* buffer, int sz);