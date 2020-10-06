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

void ClientLogin_UpdateTime(LL time, std::string sid) {
	//学生登录时更新checkin

	LL tmp=0;
	QueryStudentCheckInOut<LL>(subjectName, className, sid, tmp, "checkin");
	if (tmp != 0)return;
	UpdateStudentCheckinOut(subjectName, className, sid, time, "checkin");
}

void ClientLogout_UpdateTime(LL time, std::string sid) {
	UpdateStudentCheckinOut(subjectName, className, sid, time, "checkout");
}

void ClassOver_UpdateCheck() {

}

void ClientLogin(Data_login data,_client* client) {
	//登录还应验证该学生是否在该课堂
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
		ClientLogout_UpdateTime(TimeStamp(), client->clientInfo->sid);
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
		client->conTime = TimeStamp();
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

	std::string subject_name[] = { "math","chinese","english" };
	std::string student_name[] = { "a","b","c" };
	std::string class_name[] = { "math1","math2","math3" };
	Data_Array<std::string>classes(class_name, 3);
	//SaveClassroomSubject(classroomName, subject_name, 3);
	//SaveSubjectStudent("math", student_name, 3);
	//UpdateSubjectClass("math", classes);
	//InsertClass("math", "math2");
	//InsertClass("math", "math3");

	//UpdateStudentCheck("math", "math1", "a", -1);

	int x;
	QueryStudentCheckInOut<int>("math", "math2", "a", x, "chk");
	printf("adasdas :%d\n", x);
	Data_Student data;
	QueryStudent("math", "math1", "a", data);
	std::cout << data.attendence << std::endl;
	for (int i = 0;i < data.check.size();i++) {
		printf("%d\n", data.check[i]);
	}
	while (1) {
		AcceptCon();

		Sleep(1000);
	}
	return 0;
}