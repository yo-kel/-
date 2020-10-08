#include "request.h"

char buff[2000];

void Disconnect() {
	if (clientSocket)closesocket(clientSocket);
}

int Recv( char* buffer, int sz) {
	int result = recv(clientSocket, buffer, sz, 0);
	if (result == 0 || result == SOCKET_ERROR) {
		Disconnect();
		return false;
	}
	return true;
}




void ServerMessage(Data_Message data) {
	if (data.broadcast == 1) {
		ShowBroadcastMessage(data.name, data.message);
	}
	else {
		if (data.status == 1) {
			CreateSession(data.name);
		}
		else {
			ShowSessionMessage(data.name, data.message);
		}
	}
}

void HandleRequest() {
	while (SocketConnection)
	{
		int result = Recv(buff, 2000);
		if (result == false)return;
		std::string rawData(buff);

		Data data;
		data = DataDeserialize(rawData);

		switch (data.payload.which()) {
		case Data::Message: {
			Data_Message data_message;
			data_message = boost::get<Data_Message>(data.payload);
			ServerMessage(data_message);
			break;
		}
		}

		Sleep(1000);
	}
}