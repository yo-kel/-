#include "controller.h"


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
		ClientLogin(data_login,client);
		break;
	}
	//std::cout << buff << std::endl;
}


void ClientLogin(Data_login data,_client* client) {
	if (client->clientInfo->authentication) {
		//发送消息通知client已登录
		return;
	}
	puts("ClientLogin");
	std::string pwd;
	std::cout << pwd << std::endl;
	client->iResult = FindStudentPwd(data.sid,pwd);
	std::cout << client->iResult << std::endl;
	if (client->iResult != 0)return;
	if (pwd != data.pwd) {
		//发送消息通知client密码错误
		return;
	}
	//统计登录时间，并判断是否迟到
	client->clientInfo->authentication = 1;
	client->clientInfo->sid = data.sid;
	std::cout << data.sid << std::endl;
	client->clientInfo->position = data.position;
	client->clientInfo->loginTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch());


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


//send,recv应该在断开连接时结束对应线程
int Recv(_client* client, char* buffer, int sz) {
	if (client->con == false)return false;
	if (FD_ISSET(client->sock, &client->set)) {
		client->iResult = recv(client->sock, buffer, sz, 0);
		if (client->iResult == 0||client->iResult==SOCKET_ERROR) {
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


void Disconnect(_client* client) {
	if (client->sock)closesocket(client->sock);
	if (client->clientInfo->authentication) {
		LogOutNoti(client);//登出掉线提醒
		//统计登入登出时间
	}
	client->con = 0;
	client->iResult = -1;
	ZeroMemory(&client->clientInfo, sizeof(client->clientInfo));
}


int Accept(_client* client) {
	client->iResult = sizeof(sockaddr);
	//std::cout << "asdasdss" << client->sock << std::endl;
	client->sock = accept(serverSocket, (sockaddr*)&client->addr, &client->iResult);
	//std::cout << "asdasdss" << client->sock << std::endl;
	if (client->sock != 0 && client->sock != SOCKET_ERROR) {
		client->con = true;
		FD_ZERO(&client->set);
		FD_SET(client->sock, &client->set);
		client->conTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
		std::thread t(ClientController, client);
		t.detach();
		return true;
	}
	return false;
}

void AcceptCon() {
	for (int i = 0;i < ClientMax;i++) {
		if (client[i]->con)continue;
		int x = Accept(client[i]);
		if (x) {
			std::cout << "new client connected" << std::endl;
		}
		else break;
	}
}

void LogOutNoti(_client* client) {
	std::cout << "student "<<client->clientInfo->sid <<"has logged out"<< std::endl;
}

int main() {

	MysqlInit();
	if (ServerInit())return 0;
	std::string pwd;

	std::string subject_name[] = { "math","chinese" };
	std::string student_name[] = { "lasdas","asdasd","123asdas" };
	while (1) {
		AcceptCon();

		Sleep(1000);
	}
	return 0;
}