#ifndef ACTIVITYH
#define ACTIVITYH

#pragma once
#include "global.h"
#include "data.h"
//activity只处理本地事务和向用户端发送信息
void SendSessionMessage(_client* client, std::string message, int status);
void BroadcastMessage(std::string name, std::string message);
void SendLocalBroadcastMessage(std::string name, std::string message);
void ShowSessionMessage(std::string name, std::string message);

#endif