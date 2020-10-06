#include "request.h"

char buff[2000];

int Recv(_client* client, char* buffer, int sz) {
	if (client->con == false)return false;
	if (FD_ISSET(client->sock, &client->set)) {
		client->iResult = recv(client->sock, buffer, sz, 0);
		if (client->iResult == 0 || client->iResult == SOCKET_ERROR) {
			Disconnect(client);
			return false;
		}
		return true;
	}
	return false;
}



void ProcessClientRequest(_client* client) {
	client->iResult = Recv(client, buff, 2000);
	if (!client->iResult)return;
	std::string rawData(buff);

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
	case Data::Message:{
		Data_Message data_message;
		data_message = boost::get<Data_Message>(data.payload);
		ClientMessage(data_message, client);
	}
	}
}

int Send(_client* client, char* buffer, int sz) {
	client->iResult = send(client->sock, buffer, sz, 0);
	if (client->iResult == 0 || client->iResult == SOCKET_ERROR) {
		Disconnect(client);
		return false;
	}
	return true;
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


void ClientLogin(Data_login data, _client* client) {
	//登录还应验证该学生是否在该课堂
	if (client->clientInfo->authentication) {
		//发送消息通知client已登录
		return;
	}
	puts("ClientLogin");
	std::string pwd;
	std::cout << pwd << std::endl;
	client->iResult = FindStudentPwd(data.sid, pwd);
	std::cout << client->iResult << std::endl;
	if (client->iResult != 0)return;
	if (pwd != data.pwd) {
		//发送消息通知client密码错误
		return;
	}
	client->clientInfo->authentication = 1;
	client->clientInfo->sid = data.sid;
	std::cout << data.sid << std::endl;
	client->clientInfo->position = data.position;
	client->clientInfo->loginTime = TimeStamp();

	ClientLogin_UpdateTime(client->clientInfo->loginTime, client->clientInfo->sid);

	std::cout << "student " << data.sid << " login" << std::endl;
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
		BroadcastMessage(data.name, data.message);
		SendLocalBroadcastMessage(data.message, data.name);
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