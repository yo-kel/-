#include "server.h"




int Accept(_client* client) {
	client->iResult = sizeof(sockaddr);
	//std::cout << "asdasdss" << client->sock << std::endl;
	client->sock = accept(serverSocket, (sockaddr*)&client->addr, &client->iResult);
	//std::cout << "asdasdss" << client->sock << std::endl;
	if (client->sock != 0 && client->sock != SOCKET_ERROR) {
		client->con = true;
		FD_ZERO(&client->set);
		FD_SET(client->sock, &client->set);
		client->clientInfo.conTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
		std::thread t(ClientController, client);
		t.detach();
		return true;
	}
	return false;
}

void AcceptCon() {
	for (int i = 0;i < ClientMax;i++) {
		if (client[i]->con)continue;
		int x = Accept(client[i]);
		if (x) {
			std::cout << "new client connected" << std::endl;
		}
		else break;
	}
}

void Disconnect(_client* client) {
	if (client->sock)closesocket(client->sock);
	client->con = 0;
	client->iResult = -1;
	ZeroMemory(&client->clientInfo, sizeof(client->clientInfo));
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

	u_long mode = 1;
	ioctlsocket(serverSocket, FIONBIO, &mode);
	//mode=1:非阻塞

	return 0;
}

void DataInit() {
	for (int i = 0;i < ClientMax;i++) {
		client[i] = new _client;
		ZeroMemory(client[i], sizeof(*(client[i])));
		//std::cout << client[i]->sock << std::endl;
	}
}

int main() {
	DataInit();
	if (ServerInit())return 0;
	
	while (1) {
		AcceptCon();
		Sleep(1000);
	}
	return 0;
}