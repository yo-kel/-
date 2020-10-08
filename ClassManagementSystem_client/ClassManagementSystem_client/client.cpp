#include "client.h"

SOCKET clientSocket;
int SocketConnection = 0;

struct ServerInfo {
	LL conTime;
};

struct _server {
	bool con;
	sockaddr_in addr;
	SOCKET sock;
	fd_set set;
	int iResult;
	ServerInfo serverInfo;
};


int ClientInit() {
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
	ser.sin_port = htons(ClientPort);
	ser.sin_addr.s_addr = inet_addr(ServerAddress);
	memcpy(&addr, &ser, sizeof(SOCKADDR_IN));

	clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (clientSocket == INVALID_SOCKET) {
		std::cout << "invalid socket" << std::endl;
		return -1;
	}
	else if (clientSocket == SOCKET_ERROR) {
		std::cout << "socket error" << std::endl;
		return -1;
	}
	else {
		std::cout << "socket established" << std::endl;
	}

	iResult = connect(clientSocket, &addr, sizeof(addr));
	if (iResult==SOCKET_ERROR){
		std::cout << "SERVER UNAVAILABLE" << std::endl;
		return -1;
	}
	else {
		SocketConnection = 1;
		std::cout << "Connected to Server: " << ServerAddress << std::endl;
		memcpy(&ser, &addr, sizeof(SOCKADDR));
	}
	if (clientSocket == INVALID_SOCKET) {
		std::cout << "wrong" << std::endl;
	}
}