#include "controller.h"

SOCKET serverSocket;
_client* client[ClientMax];

char buff[500];

void ProcessClientRequest(_client* client) {
	client->iResult = Recv(client, buff, 500);
	if (!client->iResult)return;
	std::string rawData(buff);

	Data data;
	data = DataDeserialize(rawData);
	switch (data.payload.which()) {
	case Data::Login:
		Data_login data_login;
		data_login = boost::get<Data_login>(data.payload);
		ClientLogin(data_login);
		break;
	}
	//std::cout << buff << std::endl;
}


void ClientLogin(Data_login data) {
	puts("ClientLogin");

}

void ClientController(_client* client) {
	while (client->con) {
		ProcessClientRequest(client);
		//Sleep(1000);
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
