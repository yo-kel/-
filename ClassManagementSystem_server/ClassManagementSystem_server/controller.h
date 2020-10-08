#pragma once
#ifndef controllerH
#define controllerH



//controller处理来自网络的请求与数据库的操作

#include "global.h"
#include "data.h"
#include "mysql.h"
#include "server.h"
#include "activity.h"
#include "request.h"
#include "fileio.h"

#pragma comment(lib,"Ws2_32.lib")





void ClientController(_client* client);


#endif // !controller