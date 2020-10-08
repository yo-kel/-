#include "activity.h"

void SendData(Data& data) {
	std::string dataSerial;
	dataSerial = DataSerialize(data);

	int n = dataSerial.length();
	//strcpy(sendBuff, dataSerial.c_str());

	send(clientSocket, dataSerial.c_str(), n + 1, 0);
}

void ClientLogin(std::string sid, std::string pwd, std::string position) {
	Data data;
	data.payload = Data_login(sid, pwd, position);
	SendData(data);
}

void SendSessionMessage( std::string message, int status) {
	Data data;
	data.payload = Data_Message(status, 0, message, studentName);
	SendData(data);
}

void SendBroadcastMessage(std::string message) {
	Data data;
	data.payload = Data_Message(1, 1, message, studentName);
	SendData(data);
}

void ShowBroadcastMessage(std::string name, std::string message) {
	std::cout << "broadcast message " << name << " : " << message << std::endl;

}

void CreateSession(std::string name) {
	std::cout << "create session with "<<name<< std::endl;
}


void ShowSessionMessage(std::string name, std::string message) {
	std::cout << "session message " << name << " : " << message << std::endl;

}