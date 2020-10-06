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