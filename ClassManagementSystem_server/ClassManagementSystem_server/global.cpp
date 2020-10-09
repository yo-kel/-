#include "global.h"
SOCKET serverSocket;
_client* client[ClientMax];

std::string classroomName="1006";
std::string subjectName = "math";
std::string className = "math1";

LL TimeStamp() {
	return 0;
	/*auto now = std::chrono::system_clock::now();
	auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
	auto epoch = now_ms.time_since_epoch();
	auto value = std::chrono::duration_cast<std::chrono::milliseconds>(epoch);*/
	//return value.count();
}
int ReadChunkHeader(char* buffer,int offset) {
	//puts("puts");
	//std::cout << buffer << std::endl;
	char tmp[10];
	for (int i = 0;i < 10;i++) {
		tmp[i] = buffer[offset + i];
		if (tmp[i] == '\0')return 0;
	}
	std::string bytes(tmp, 10);
	//std::cout << bytes << std::endl;
	int num = std::stoi(bytes);
	return num;
}


std::string WriteChunkHeader(int x) {
	std::string num = std::to_string(x);
	int len = num.length();
	if (len < 10) {
		std::string leading(10 - len, '0');
		num = leading + num;
	}
	return num;
}
std::string DataSerialChunk(std::string dataSerial) {
	int n = dataSerial.length();
	//std::cout << "|" << dataSerial << "|" << std::endl;
	dataSerial = WriteChunkHeader(n) + dataSerial;
	n = dataSerial.length();
	if (n < Chunk_Size) {
		std::string tail(Chunk_Size - n, '_');
		dataSerial = dataSerial + tail;
	}
	return dataSerial;
}