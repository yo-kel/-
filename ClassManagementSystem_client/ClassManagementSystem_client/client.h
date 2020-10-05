#ifndef CLIENTH
#define CLIENTH


#pragma once

#include "global.h"
#include "data.h"


#define ClientPort 6666
#define BufferMax 500
#define ServerAddress "127.0.0.1"//考虑到实际使用，应可以修改


extern SOCKET clientSocket;

int ClientInit();
#endif // !