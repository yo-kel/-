#include "controller.h"




	//std::cout << buff << std::endl;


void ClassOver_UpdateCheck() {//

}


//send,recv应该在断开连接时结束对应线程


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


int main() {

	MysqlInit();
	if (ServerInit())return 0;

	//std::string subject_name[] = { "math","chinese","english" };
	//std::string student_name[] = { "a","b","c" };
	//std::string class_name[] = { "math1","math2","math3" };
	//Data_Array<std::string>classes(class_name, 3);
	//SaveClassroomSubject(classroomName, subject_name, 3);
	//SaveSubjectStudent("math", student_name, 3);
	//UpdateSubjectClass("math", classes);
	//InsertClass("math", "math2");
	//InsertClass("math", "math3");

	//UpdateStudentCheck("math", "math1", "a", -1);

	//int x;
	//QueryStudentCheckInOut<int>("math", "math2", "a", x, "chk");
	//printf("adasdas :%d\n", x);
	//Data_Student data;
	//QueryStudent("math", "math1", "a", data);
	//std::cout << data.attendence << std::endl;
	//for (int i = 0;i < data.check.size();i++) {
	//	printf("%d\n", data.check[i]);
	//}

	//std::cout << SplicePathString(3, subjectName, std::string("Bank"), std::string("1000")) << std::endl;
	//std::string path = "." + SplicePathString(4, std::string("subject"), subjectName, std::string("bank"), std::string("1001"));
	//std::vector<std::string >S;
	//AllFileNames(path, S);
	//WriteToFile(path, "1001.hmwk", "hello world123");
	//std::cout << SplicePathString(3, subjectName, "Bank", "1000") << std::endl;
	while (1) {
		AcceptCon();

		Sleep(1000);
	}
	return 0;
}