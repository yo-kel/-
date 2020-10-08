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
	case Data::Message: {
		Data_Message data_message;
		data_message = boost::get<Data_Message>(data.payload);
		ClientMessage(data_message, client);
		break;
	}
	case Data::Ans:{
		Data_Ans data_ans;
		data_ans = boost::get<Data_Ans>(data.payload);
		ClientAns(data_ans, client);
		break;
	}
	}
}


void ClientAns(Data_Ans data, _client* client) {
	//���𰸱��浽��Ӧ���õĶ�Ӧѧ����������ѡ�����Զ�����
	data.checked = Data_Ans::unchecked;
	SaveStuAns(data.title, client->clientInfo->sid, data);
	if (data.type == "choice" || data.type == "blank")AutoMarkStuHmwk(data.title, client->clientInfo->sid);
}

void ClientLogin(Data_login data, _client* client) {
	//��¼��Ӧ��֤��ѧ���Ƿ��ڸÿ���
	if (client->clientInfo->authentication) {
		//������Ϣ֪ͨclient�ѵ�¼
		return;
	}
	puts("ClientLogin");
	std::string pwd;
	std::cout << pwd << std::endl;
	client->iResult = FindStudentPwd(data.sid, pwd);
	std::cout << client->iResult << std::endl;
	if (client->iResult != 0)return;
	if (pwd != data.pwd) {
		//������Ϣ֪ͨclient�������
		return;
	}
	client->clientInfo->authentication = 1;
	client->clientInfo->sid = data.sid;
	std::cout << data.sid << std::endl;
	client->clientInfo->position = data.position;
	client->clientInfo->loginTime = TimeStamp();
	//std::cout << "sdadads" << TimeStamp() << std::endl;
	ClientLogin_UpdateTime(client->clientInfo->loginTime, client->clientInfo->sid);

	std::cout << "student " << data.sid << " login" << std::endl;
	//ѧ��ֻ�ܽ��뵱ǰ�����Ŀ���,ѧ����¼�ǳ�ֻ���ǵ�һ�ε�¼�����һ�εǳ�
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
		std::cout << data.name << " " << data.message << std::endl;
		BroadcastMessage(data.name, data.message);
		SendLocalBroadcastMessage(data.name, data.message);
	}
	else {
		if (data.status == 1)SendSessionMessage(client, "create session", 1);//ѧ����Ҳ���������ʦ�˷�������
		else ShowSessionMessage(data.name, data.message);
	}
}

void ClientQuestion(Data_Question data,_client* client ) {
	std::cout << "student " + data.name + " " + data.sid + " at " + data.position + "asks a question" << std::endl;
	std::cout << data.content << std::endl;
}
