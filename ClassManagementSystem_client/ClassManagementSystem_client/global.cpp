#include "global.h";

std::string clientPosition="Row 1,Col 2";
std::string studentName = "student";
std::string sid = "123";
std::string pwd = "456";

LPCWSTR ConvertStringToLpcwstr(std::string s) {
	std::wstring stemp = std::wstring(s.begin(), s.end());
	LPCWSTR sw = stemp.c_str();
	return sw;
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
int ReadChunkHeader(char* buffer, int offset) {
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