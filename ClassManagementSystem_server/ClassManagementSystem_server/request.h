#ifndef REQUESTH
#define REQUESTH



#pragma once
#include "global.h"
#include "data.h"
#include "mysql.h"
#include "activity.h"
//request 处理来自客户端的信息，调用相应的activity

int Recv(_client* client, char* buffer, int sz);
void ClientLogin(Data_login data, _client* client);
void ClientQuestion(Data_Question data, _client* client);
void ClientMessage(Data_Message data, _client* client);
void ClientAns(Data_Ans data, _client* client);

#endif // !REQUESTH


