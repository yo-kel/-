#ifndef ACTIVITYH
#define ACTIVITYH

#pragma once
#include "global.h"
#include "data.h"
#include "mysql.h"
#include "fileio.h"
//activity只处理本地事务和向用户端发送信息

int Send(_client* client, const char* buffer, int sz);
void Disconnect(_client* client);
void LogOutNoti(_client* client);


void ClientLogout_UpdateTime(LL time, std::string sid);
void ClientLogin_UpdateTime(LL time, std::string sid);

void SendSessionMessage(_client* client, std::string message, int status);


void BroadcastData(Data data);
void BroadcastMessage(std::string name, std::string message);
int BroadcastHmwk(std::string title);
void SendLocalBroadcastMessage(std::string name, std::string message);
void ShowSessionMessage(std::string name, std::string message);

template<typename T>
int ReadBank(std::string title, std::string type, T& res);
//从题库中读取试题或答案

template<typename T>
int SaveBank(std::string title, std::string type, T res);
//保存试题或答案到题库

int ReadStuAns(std::string title, std::string sid, Data_Ans& res);

int SaveStuAns(std::string title, std::string sid, Data_Ans res);

int AutoMarkStuHmwk(std::string title, std::string sid);


#endif