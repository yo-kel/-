#ifndef ACTIVITYH
#define ACTIVITYH

#pragma once

#include "global.h"
#include "data.h"
#include "client.h"

void SendData(Data& data);
void SendLogin(std::string sid, std::string pwd);
void ClientLogin(std::string sid, std::string pwd, std::string position);
//用户登录
void ShowBroadcastMessage(std::string name, std::string message);
//发送广播消息
void CreateSession(std::string name);
//
void ShowSessionMessage(std::string name, std::string message);

void SendQuestion(std::string question);

void SendSessionMessage(std::string message, int status);
void SendBroadcastMessage(std::string message);




#endif // !ACTIVITYH


