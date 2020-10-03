#pragma once
#include <stdio.h>
#include <winsock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <iostream>

#pragma comment(lib,"Ws2_32.lib")

#define ServerPort 6666
#define ClientMax 105

struct ClientInfo {

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
_client* clinet[ClientMax];