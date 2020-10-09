#include "request.h"

char buff[Buffer_Size];

int Recv(_client* client, char* buffer, int sz) {
	if (client->con == false)return false;
	if (FD_ISSET(client->sock, &client->set)) {
		client->iResult = recv(client->sock, buffer, sz, 0);
		if (client->iResult == 0 || client->iResult == SOCKET_ERROR) {
			Disconnect(client);
			return false;
		}
		return client->iResult;
	}
	return false;
}



void ProcessClientRequest(_client* client) {
	client->iResult = Recv(client, buff, Buffer_Size);
	if (!client->iResult)return;
	std::cout << buff << std::endl;
	std::cout << client->iResult << std::endl;
	int sz = ((client->iResult + 1) / Chunk_Size) * Chunk_Size;
	for (int i = 0, index = 0;index < sz;i++, index += Chunk_Size) {
		int offset = index * sizeof(CHAR);
		//printf("->>%d %d\n", i, offset);
		int len = ReadChunkHeader(buff,offset);

		if (len == 0)break;
		std::string rawData(buff + offset + 10 * sizeof(CHAR), len);
		Data data;
		data = DataDeserialize(rawData);
		switch (data.payload.which()) {
		case Data::Login: {
			Data_login data_login;
			data_login = boost::get<Data_login>(data.payload);
			ClientLogin(data_login, client);
			break;
		}
		case Data::Question: {
			Data_Question data_question;
			data_question = boost::get<Data_Question>(data.payload);
			ClientQuestion(data_question, client);
			break;
		}
		case Data::Message: {
			Data_Message data_message;
			data_message = boost::get<Data_Message>(data.payload);
			ClientMessage(data_message, client);
			break;
		}
		case Data::Ans: {
			Data_Ans data_ans;
			data_ans = boost::get<Data_Ans>(data.payload);
			ClientAns(data_ans, client);
			break;
		}
		case Data::File: {
			Data_File data_file;
			data_file = boost::get<Data_File>(data.payload);
			ClientFile(data_file, client);
		}
		}
	}
}

void ClientFile(Data_File data, _client* client) {
	std::string path = "." + SplicePathString(4, std::string("subject"),
		subjectName, className, client->clientInfo->sid);
	CreateDir(path);
	std::ofstream file(path+data.fileName, std::ios::binary | std::ios_base::app);
	if (file.is_open()) {
		file.write(data.fileBytes, data.sz);
	}

}

void ClientAns(Data_Ans data, _client* client) {
	//将答案保存到对应课堂的对应学生，并对填选类型自动批改
	data.checked = Data_Ans::unchecked;
	SaveStuAns(data.title, client->clientInfo->sid, data);
	if (data.type == "choice" || data.type == "blank")AutoMarkStuHmwk(data.title, client->clientInfo->sid);
}

void ClientLogin(Data_login data, _client* client) {
	//登录还应验证该学生是否在该课堂
	if (client->clientInfo->authentication) {
		//发送消息通知client已登录
		return;
	}
	puts("Client attempt to Login");
	std::string pwd;
	std::cout << pwd << std::endl;
	client->iResult = FindStudentPwd(data.sid, pwd);
	std::cout << client->iResult << std::endl;
	if (client->iResult != 0)return;
	if (pwd != data.pwd) {
		puts("Client Login failed");
		//发送消息通知client密码错误
		return;
	}
	puts("Client Login successfully");
	std::cout << "student " << data.sid << " login" << std::endl;
	client->clientInfo->authentication = 1;
	client->clientInfo->sid = data.sid;
	//std::cout << data.sid << std::endl;
	client->clientInfo->position = data.position;
	client->clientInfo->loginTime = TimeStamp();
	//std::cout << "sdadads" << TimeStamp() << std::endl;
	ClientLogin_UpdateTime(client->clientInfo->loginTime, client->clientInfo->sid);
	//puts("finished");
	//学生只能进入当前开启的课堂,学生登录登出只考虑第一次登录和最后一次登出
	//client->clientInfo.classroom = ;
	//client->clientInfo.subject = ;
}

void ClientController(_client* client) {
	while (client->con) {
		ProcessClientRequest(client);
		Sleep(1000);
	}
}

void ClientMessage(Data_Message data, _client* client) {
	if (data.broadcast) {
		//std::cout << data.name << " " << data.message << std::endl;
		BroadcastMessage(data.name, data.message);
		SendLocalBroadcastMessage(data.name, data.message);
	}
	else {
		if (data.status == 1)SendSessionMessage(client, "create session", 1);//学生端也可主动向教师端发起申请
		else ShowSessionMessage(data.name, data.message);
	}
}

void ClientQuestion(Data_Question data,_client* client ) {
	std::cout << "student " + data.name + " " + data.sid + " at " + data.position + "asks a question" << std::endl;
	std::cout << data.content << std::endl;
}
