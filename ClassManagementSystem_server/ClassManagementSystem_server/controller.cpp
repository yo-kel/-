#include "controller.h"

SOCKET serverSocket;
_client* client[ClientMax];

void ProcessClientRequest(_client* client) {
	
}

void ClientController(_client* client) {
	while (1) {
		ProcessClientRequest(client);
		Sleep(1000);
	}
}

//send,recv应该在断开连接时结束对应线程
int Recv(_client* client, char* buffer, int sz) {
	if (FD_ISSET(client->sock, &client->set)) {
		client->iResult = recv(client->sock, buffer, sz, 0);
		if (client->iResult == 0) {
			Disconnect(client);
			return false;
		}
		return true;
	}
	return false;
}

int Send(_client* client, char* buffer, int sz) {
	client->iResult = send(client->sock, buffer, sz, 0);
	if (client->iResult == 0 || client->iResult == SOCKET_ERROR) {
		Disconnect(client);
		return false;
	}
	return true;
}
