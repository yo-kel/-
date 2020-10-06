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

#define LL long long

#pragma comment(lib,"Ws2_32.lib")
using namespace std::chrono;

extern std::string clientPosition;

