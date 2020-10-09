#pragma once

#include <stdio.h>
#include <winsock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <sstream>
#include <string>
#include <chrono>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <thread>

#define LL long long
#define Buffer_Size 51200
#define Chunk_Size 5120
#define Part_Size 4096

#pragma comment(lib,"Ws2_32.lib")
using namespace std::chrono;

extern std::string clientPosition;
extern std::string sid;
extern std::string studentName;
extern std::string pwd;

LPCWSTR ConvertStringToLpcwstr(std::string s);
int ReadChunkHeader(char* buffer, int offset);
std::string WriteChunkHeader(int x);
std::string DataSerialChunk(std::string dataSerial);