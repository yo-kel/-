#ifndef ACTIVITYH
#define ACTIVITYH

#pragma once

#include "global.h"
#include "data.h"
#include "client.h"

void SendData(Data& data);
void SendLogin(std::string sid, std::string pwd);
void ClientLogin(std::string sid, std::string pwd, std::string position);
void ShowBroadcastMessage(std::string name, std::string message);
void CreateSession(std::string name);
void ShowSessionMessage(std::string name, std::string message);


void SendSessionMessage( std::string message, int status);
void SendBroadcastMessage(std::string name);




#endif // !ACTIVITYH


