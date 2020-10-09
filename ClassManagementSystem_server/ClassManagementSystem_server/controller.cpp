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


void HandleActivity() {

	//std::cout << "load class roomsubject" << std::endl;

	int x;
	std::cin >> x;
	Data_Array<std::string>classroomSubject;
	QueryClassroomSubject(classroomSubject);
	std::cout << "->classroom " + classroomName + " has subject:" << std::endl;
	for (int i = 0;i < classroomSubject.S.size();i++) {
		std::cout << classroomSubject.S[i] << std::endl;
	}


	//std::cout << u8"导入 教室的科目" << std::endl;
	std::cin >> x;
	std::string subject[] = { "eating", "gaming", "sleeping" };
	std::cout << "->update classroom " << classroomName << "subject" << std::endl;
	std::cout <<"->"<< subject[0] << " " << subject[1] << " " << subject[2] << std::endl;
	SaveClassroomSubject(classroomName, subject, 3);


	//std::cout << u8"导出 教室的科目" << std::endl;
	std::cin >> x;
	//Data_Array<std::string>classroomSubject;
	QueryClassroomSubject(classroomSubject);
	std::cout << "->classroom " + classroomName + " has subject:" << std::endl;
	for (int i = 0;i < classroomSubject.S.size();i++) {
		std::cout <<"->"<< classroomSubject.S[i] << std::endl;
	}


	//std::cout << u8"导出 科目课堂" << std::endl;
	std::cin >> x;
	Data_Array<std::string>subjectClass;
	QuerySubjectClass(subjectName, subjectClass);
	std::cout << "->Subject " + subjectName + " has class:" << std::endl;
	for (int i = 0;i < subjectClass.S.size();i++) {
		std::cout << "->" << subjectClass.S[i] << std::endl;
	}


	//std::cout << u8"导出 科目学生" << std::endl;
	std::cin >> x;
	subjectName = "math";
	Data_Array<std::string>subjectStudent;
	QuerySubjectStudent(subjectName, subjectStudent);
	std::cout << "->Subject " + subjectName + " has student:" << std::endl;
	for (int i = 0;i < subjectStudent.S.size();i++) {
		std::cout << "->" << subjectStudent.S[i] << std::endl;
	}


	//std::cout << u8"修改 当前科目" << std::endl;

	std::cout << "change current subject" << std::endl;
	std::string str;
	std::cin >> str;
	subjectName = str;


	//std::cout << u8"导入 科目学生" << std::endl;
	std::cin >> x;
	std::string student[] = { "p1", "p2", "p3" };
	std::cout << "->update subject " << subjectName << "student" << std::endl;
	std::cout << "->" << student[0] << " " << student[1] << " " << student[2] << std::endl;
	SaveSubjectStudent(subjectName, student, 3);


	//std::cout << u8"导出 科目学生" << std::endl;
	std::cin >> x;
	//Data_Array<std::string>subjectStudent;
	QuerySubjectStudent(subjectName, subjectStudent);
	std::cout << "->Subject " + subjectName + " has student:" << std::endl;
	for (int i = 0;i < subjectStudent.S.size();i++) {
		std::cout <<"->"<< subjectStudent.S[i] << std::endl;
	}

	std::cout << "create class" << std::endl;
	std::cin >> str;
	className = str;
	InsertClass(subjectName,className);


	//std::cout << u8"导出 科目课堂" << std::endl;
	std::cin >> x;
	Data_Array<std::string>newSubjectClass;
	QuerySubjectClass(subjectName, newSubjectClass);
	std::cout << "->Subject " + subjectName + " has class:" << std::endl;
	for (int i = 0;i < newSubjectClass.S.size();i++) {
		std::cout << "->" << newSubjectClass.S[i] << std::endl;
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
	std::thread t(HandleActivity);
	t.detach();
	while (1) {
		AcceptCon();

		Sleep(1000);
	}
	return 0;
}