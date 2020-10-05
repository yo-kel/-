#include "controller.h";


void SendData(Data& data) {
	std::string dataSerial;
	dataSerial = DataSerialize(data);

	int n = dataSerial.length();
	strcpy(sendBuff, dataSerial.c_str());

	send(clientSocket, sendBuff, n + 1, 0);
}

void SendLogin(std::string sid, std::string pwd) {
	Data data;
	data.payload = Data_login(sid, pwd, clientPosition);
	SendData(data);
}

int main() {
	ClientInit();


	
	Data data;
	data.payload = Data_login("123", "456", "1r2c");
	std::string dataSerial;
	dataSerial = DataSerialize(data);

	int n = dataSerial.length();
	char buff[500];
	strcpy(buff, dataSerial.c_str());
	//std::cout << dataSerial << std::endl;
	send(clientSocket, buff, n + 1, 0);
	//send(clientSocket, buff, n + 1, 0);
	//Data newdata;
	//puts("------");
	//HandleRequest(dataSerial);
	//std::cout << newdata.position << std::endl;
	while (1);
}