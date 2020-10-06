#ifndef REQUESTH
#define REQUESTH



#pragma once
#include "global.h"
#include "data.h"
#include "mysql.h"
#include "activity.h"
//request 处理来自客户端的信息，调用相应的activity

int Send(_client* client, char* buffer, int sz);
int Recv(_client* client, char* buffer, int sz);
void Disconnect(_client* client);
void ClientLogin(Data_login data, _client* client);
void ClientQuestion(Data_Question data, _client* client);
void ClientMessage(Data_Message data, _client* client);
void LogOutNoti(_client* client);
void ClientLogout_UpdateTime(LL time, std::string sid);
void ClientLogin_UpdateTime(LL time, std::string sid);
#endif // !REQUESTH


