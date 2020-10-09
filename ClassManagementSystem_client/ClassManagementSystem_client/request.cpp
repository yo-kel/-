#include "request.h"

char buff[Buffer_Size];

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
		int result = Recv(buff, Buffer_Size);
		if (result == false)return;
		int sz = (result / Chunk_Size + 1) * Chunk_Size;
		for (int i = 0, index = 0;index < sz;i++, index += Chunk_Size) {
			int offset = index * sizeof(CHAR);
			//printf("->>%d %d\n", i, offset);
			int len = ReadChunkHeader(buff, offset);

			if (len == 0)break;
			//puts("YES");
			std::string rawData(buff + offset + 10 * sizeof(CHAR), len);

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
		}

		Sleep(1000);
	}
}