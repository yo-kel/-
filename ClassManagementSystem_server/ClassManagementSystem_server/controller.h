#pragma once
#ifndef controllerH
#define controllerH



//controller处理来自网络的请求与数据库的操作

#include "global.h"

#pragma comment(lib,"Ws2_32.lib")


#define ServerPort 6666
#define ClientMax 105

struct ClientInfo {
	milliseconds conTime;
	int authentication;
};

struct _client {
	bool con;
	sockaddr_in addr;
	SOCKET sock;
	fd_set set;
	int iResult;
	ClientInfo clientInfo;
};




extern SOCKET serverSocket;
extern _client* client[ClientMax];




void ClientController(_client* client);
int Send(_client* client, char* buffer, int sz);
int Recv(_client* client, char* buffer, int sz);
void Disconnect(_client* client);




#endif // !controller