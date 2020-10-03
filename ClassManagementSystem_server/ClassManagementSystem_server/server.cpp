#include "server.h"

int AcceptCon(_client* client) {
	client->iResult = sizeof(sockaddr);
	client->sock = accept(serverSocket, (sockaddr*)&client->addr, &client->iResult);
	if (client->sock != 0 && client->sock != SOCKET_ERROR) {
		client->con = true;
		FD_ZERO(&client->set);
		FD_SET(client->sock, &client->set);
	}
}

int ServerInit() {
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

	iResult = bind(serverSocket, &addr, sizeof(addr));
	if (iResult != 0) {
		std::cout << "socket binding error" << std::endl;
		return -1;
	}
	else {
		std::cout << "socket bound to port:" << ServerPort << std::endl;
	}

	iResult = listen(serverSocket, 10);

	u_long mode = 1;
	ioctlsocket(serverSocket, FIONBIO, &mode);

	return 0;
}

int main() {
	if (!ServerInit())return 0;
	while (1) {
		


		Sleep(500);
	}
	return 0;
}