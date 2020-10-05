#include "server.h"








int ServerInit() {

	for (int i = 0;i < ClientMax;i++) {
		client[i] = new _client;
		ZeroMemory(client[i], sizeof(*(client[i])));
		client[i]->clientInfo = new ClientInfo;
		ZeroMemory(client[i]->clientInfo, sizeof((*client[i]).clientInfo));
		client[i]->clientInfo->authentication = 0;
		//std::cout << client[i]->clientInfo->authentication << std::endl;
	}

	WSADATA wsaData;
	int iResult;
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		std::cout << "WSAStartup failed " << iResult << std::endl;
		return -1;
	}
	
	sockaddr_in ser;
	sockaddr addr;
	ser.sin_family = AF_INET;
	ser.sin_port = htons(ServerPort);
	ser.sin_addr.s_addr = INADDR_ANY;
	memcpy(&addr, &ser, sizeof(SOCKADDR_IN));

	serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverSocket == INVALID_SOCKET) {
		std::cout << "invalid socket" << std::endl;
		return -1;
	}
	else if (serverSocket == SOCKET_ERROR) {
		std::cout << "socket error" << std::endl;
		return -1;
	}
	else {
		std::cout << "socket established" << std::endl;
	}

	int bReuseaddr = true;
	setsockopt(serverSocket,SOL_SOCKET, SO_REUSEADDR,(char*) &bReuseaddr, sizeof(bReuseaddr));
	//SO_REUSEADDR：使被释放的端口可以被立即使用

	iResult = bind(serverSocket, &addr, sizeof(addr));
	if (iResult != 0) {
		std::cout << "socket binding error" << std::endl;
		return -1;
	}
	else {
		std::cout << "socket bound to port:" << ServerPort << std::endl;
	}

	iResult = listen(serverSocket, 10);

	//u_long mode = 1;
	//ioctlsocket(serverSocket, FIONBIO, &mode);
	//mode=1:非阻塞

	return 0;
}

