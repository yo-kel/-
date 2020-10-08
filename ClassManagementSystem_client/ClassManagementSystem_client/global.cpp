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