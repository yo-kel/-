#include "activity.h"


void SendSessionMessage(_client* client, std::string message, int status) {
	Data data;
	data.payload = Data_Message(status, 0, message,"teacher");
	std::string dataSerial;
	dataSerial = DataSerialize(data);
	int n = dataSerial.length();
	Send(client, dataSerial.c_str(), n + 1);
}


void ClientLogin_UpdateTime(LL time, std::string sid) {
	//学生登录时更新checkin

	LL tmp = 0;
	QueryStudentCheckInOut<LL>(subjectName, className, sid, tmp, "checkin");
	if (tmp != 0)return;
	UpdateStudentCheckinOut(subjectName, className, sid, time, "checkin");
}

void ClientLogout_UpdateTime(LL time, std::string sid) {
	UpdateStudentCheckinOut(subjectName, className, sid, time, "checkout");
}
void Disconnect(_client* client) {
	if (client->sock)closesocket(client->sock);
	if (client->clientInfo->authentication) {
		ClientLogout_UpdateTime(TimeStamp(), client->clientInfo->sid);
		LogOutNoti(client);//登出掉线提醒
		//统计登入登出时间
	}
	client->con = 0;
	client->iResult = -1;
	ZeroMemory(&client->clientInfo, sizeof(client->clientInfo));
}
void LogOutNoti(_client* client) {
	std::cout << "student " << client->clientInfo->sid << "has logged out" << std::endl;
}

int Send(_client* client, const char* buffer, int sz) {
	client->iResult = send(client->sock, buffer, sz, 0);
	if (client->iResult == 0 || client->iResult == SOCKET_ERROR) {
		Disconnect(client);
		return false;
	}
	return true;
}


char file_data[Chunk_Size];

void SendFileThread(std::string fileName) {
	FILE* file;
	file = fopen(fileName, "rb");
	size_t nbytes = 0;
	Data data;
	Data_File;
	while ((nbytes = fread(file_data, , sizeof(char), Chunk_Size, file)) > 0) {
		
		Send()
	}
}

void SendFile(std::string fileName) {

}

void BroadcastData(Data data) {
	for (int i = 0;i < ClientMax;i++) {
		if (!client[i]->con || !client[i]->clientInfo->authentication)continue;
		//puts("YES");
		//std::cout << client[i]->clientInfo->sid << std::endl;
		std::string dataSerial;
		dataSerial = DataSerialize(data);
		int n = dataSerial.length();
		Send(client[i], dataSerial.c_str(), n + 1);
	}
}

void BroadcastData(std::string dataSerial) {
	for (int i = 0;i < ClientMax;i++) {
		if (!client[i]->con || !client[i]->clientInfo->authentication)continue;
		//puts("YES");
		//std::cout << client[i]->clientInfo->sid << std::endl;
		int n = dataSerial.length();
		Send(client[i], dataSerial.c_str(), n + 1);
	}
}

void BroadcastMessage(std::string name, std::string message) {
	Data data;
	data.payload = Data_Message(1, 1, message, name);
	BroadcastData(data);
}

int BroadcastHmwk(std::string title) {
	//从题库里寻找名为title的题目
	Data_Hmwk data_hmwk;
	if (ReadBank<Data_Hmwk>(title, "hmwk", data_hmwk) != 0)return -1;
	Data data;
	data.payload = data_hmwk;
	BroadcastData(data);

	return 0;
}

void SendLocalBroadcastMessage(std::string name, std::string message) {
	std::cout << "broadcast message " << name << " : " << message << std::endl;
}

void ShowSessionMessage(std::string name, std::string message) {
	std::cout << "Session message " << name << " : " << message << std::endl;
}


template<typename T>
int ReadBank(std::string title, std::string type, T& res) {

	std::string tmp;
	if (type == "ans")tmp = ".ans";
	else if (type == "hmwk")tmp = ".hmwk";
	std::string path = "." + SplicePathString(4, std::string("subject"),
		subjectName, std::string("Bank"), title);

	std::string content;
	if (ReadFromFile(path, title + tmp, content) == -1) {
		std::cout << "can't find the file" << std::endl;
		return -1;
	}
	Data data = DataDeserialize(content);
	switch (data.payload.which()) {
	case Data::Ans: {
		//res = boost::get<Data_Ans>(data.payload);
		break;
	}
	case Data::Hmwk: {
		//res = boost::get<Data_Hmwk>(data.payload);
		break;
	}
	default: {
		return -1;
	}
	}
	return 0;
}
template int ReadBank<Data_Ans>(std::string title, std::string type, Data_Ans& res);
template int ReadBank<Data_Hmwk>(std::string title, std::string type, Data_Hmwk& res);

template<typename T>
int SaveBank(std::string title, std::string type, T res) {

	std::string tmp;
	if (type == "ans")tmp = ".ans";
	else if (type == "hmwk")tmp = ".hmwk";
	std::string path = "." + SplicePathString(4, std::string("subject"),
		subjectName, std::string("Bank"), title);


	Data data;
	data.payload = res;
	std::string dataSerial = DataSerialize(data);
	if (WriteToFile(path, title + tmp, dataSerial) != 0)return -1;
	return 0;
}
template int SaveBank<Data_Ans>(std::string title, std::string type, Data_Ans res);
template int SaveBank<Data_Hmwk>(std::string title, std::string type, Data_Hmwk res);

int ReadStuAns(std::string title,std::string sid, Data_Ans &res) {
	std::string path = "." + SplicePathString(5, std::string("subject"),
		subjectName, className,sid,title);
	std::string dataSerial;
	if (ReadFromFile(path, title + ".ans", dataSerial)!=0)return -1;
	Data data = DataDeserialize(dataSerial);
	res = boost::get<Data_Ans>(data.payload);
	return 0;
}

int SaveStuAns(std::string title, std::string sid, Data_Ans res) {
	std::string path = "." + SplicePathString(5, std::string("subject"),
		subjectName, className, sid, title);
	Data data;
	data.payload = res;
	std::string dataSerial = DataSerialize(data);
	if (WriteToFile(path, title + ".ans", dataSerial) != -1)return -1;
	return 0;

}

int AutoMarkStuHmwk(std::string title, std::string sid) {
	Data_Ans stuAns;
	if (ReadStuAns(title, sid, stuAns) != 0)return -1;
	Data_Ans stdAns;
	if (ReadBank<Data_Ans>(title, "ans", stdAns) != 0)return -1;
	if (!(stuAns.title == stdAns.title && stuAns.type == stdAns.type && stuAns.content == stdAns.content)) {
		stuAns.checked = Data_Ans::wrong;
		SaveStuAns(title, sid, stuAns);
		//答错不得分，数据库不修改
	}
	else {
		stuAns.checked = Data_Ans::correct;
		SaveStuAns(title, sid, stuAns);
		//修改数据库得分
	}
	return 0;
}