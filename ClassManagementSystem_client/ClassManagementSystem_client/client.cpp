#include "client.h"

int Accept(_server* server) {
	server->iResult = sizeof(sockaddr);
	server->sock = accept(clientSocket, (sockaddr*)&server->addr, &server->iResult);
	if (server->sock != 0 && server->sock != SOCKET_ERROR) {
		server->con = true;
		FD_ZERO(&server->set);
		FD_SET(server->sock, &server->set);
		server->serverInfo.conTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
		return true;
	}
	return false;
}


void Disconnect(_server* server) {
	if (server->sock)closesocket(server->sock);
	server->con = 0;
	server->iResult = -1;
	ZeroMemory(&server->serverInfo, sizeof(server->serverInfo));
}


int Recv(_server* server, char* buffer, int sz) {
	if (FD_ISSET(server->sock, &server->set)) {
		server->iResult = recv(server->sock, buffer, sz, 0);
		if (server->iResult == 0) {
			Disconnect(server);
			return false;
		}
		return true;
	}
	return false;
}

int Send(_server* server, char* buffer, int sz) {
	server->iResult = send(server->sock, buffer, sz, 0);
	if (server->iResult == 0 || server->iResult == SOCKET_ERROR) {
		Disconnect(server);
		return false;
	}
	return true;
}