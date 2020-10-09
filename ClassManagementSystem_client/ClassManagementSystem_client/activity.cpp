#include "activity.h"

void SendData(Data& data) {
	std::string dataSerial;
	dataSerial = DataSerialize(data);
	//std::cout << "|" << dataSerial << "|" << std::endl;
	dataSerial = DataSerialChunk(dataSerial);
	//std::cout << dataSerial.length() << std::endl;
	send(clientSocket, dataSerial.c_str(), Chunk_Size, 0);
	//puts("");
}




void SendFileThread(std::string path,std::string fileName) {
	FILE* file;
	file = fopen((path+fileName).c_str(), "rb");
	size_t nbytes = 0;
	Data data;
	Data_File data_file;
	data_file.status=data_file.alive;
	data_file.fileName = fileName;
	while ((nbytes = fread(data_file.fileBytes, sizeof(char), Chunk_Size, file)) > 0) {
		data_file.sz = nbytes;
		data.payload = data_file;
		SendData(data);
	}
	data_file.status = data_file.finished;
	data.payload = data_file;
	SendData(data);
}

void SendFile(std::string path,std::string fileName) {
	std::thread t(SendFileThread, path, fileName);
	t.detach();
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

void SendQuestion(std::string content) {
	Data data;
	Data_Question data_question;
	data_question.position = clientPosition;
	data_question.sid = sid;
	data_question.name = studentName;
	data_question.content = content;
	data.payload = data_question;
	SendData(data);
}

void ShowSessionMessage(std::string name, std::string message) {
	std::cout << "session message " << name << " : " << message << std::endl;

}