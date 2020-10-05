#pragma once
#ifndef controllerH
#define controllerH



//controller处理来自网络的请求与数据库的操作

#include "global.h"
#include "data.h"
#include "mysql.h"
#include "server.h"

#pragma comment(lib,"Ws2_32.lib")





void ClientController(_client* client);
int Send(_client* client, char* buffer, int sz);
int Recv(_client* client, char* buffer, int sz);
void Disconnect(_client* client);
void ClientLogin(Data_login data,_client* client);
void LogOutNoti(_client* client);



#endif // !controller