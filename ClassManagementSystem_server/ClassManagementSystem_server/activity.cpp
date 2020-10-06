#include "activity.h"


void SendSessionMessage(_client* client, std::string message, int status) {
	Data data;
	data.payload = Data_Message(status, 0, message,"teacher");
	std::string dataSerial;
	dataSerial = DataSerialize(data);
	int n = dataSerial.length();
	send(client->sock, dataSerial.c_str(), n + 1, 0);
}

void BroadcastMessage(std::string message, std::string name) {
	Data data;
	data.payload = Data_Message(1, 1, message, name);
	for (int i = 0;i < ClientMax;i++) {
		if (!client[i]->clientInfo->authentication)continue;
		std::string dataSerial;
		dataSerial = DataSerialize(data);
		int n = dataSerial.length();
		send(client[i]->sock, dataSerial.c_str(), n + 1, 0);
	}
}

void SendLocalBroadcastMessage(std::string name, std::string message) {
	std::cout << "broadcast message " << name << " : " << message << std::endl;
}


void ShowSessionMessage(std::string name, std::string message) {
	std::cout << "Session message " << name << " : " << message << std::endl;
}