#include "controller.h";



void HandleActivity() {
	std::string input;
	std::getline(std::cin, input);
	std::stringstream stream(input);
	while (std::getline(std::cin, input),!input.empty()){
		if (input == "Edit Sid") {

		}

	}
}

int main() {

    //std::string file = ;
    /*LPCWSTR str = L"bitmap.bmp";
	HBITMAP hbitmap = ScreenShot();
    CreateBMPFile(L"bitmap.bmp", hbitmap);
    return 0;*/
	ClientInit();
	ClientLogin("123", "456", clientPosition);//应在handleActivity中，由用户确认执行


	if (SocketConnection) {//创建新线程处理请求
		std::thread t(HandleRequest);
		t.detach();
	}

	int i = 1;
	while (1) {
		//HandleActivity();
		SendBroadcastMessage("hello world"+std::to_string(i));
		Sleep(1000);
		i++;
	}
	return 0;
}