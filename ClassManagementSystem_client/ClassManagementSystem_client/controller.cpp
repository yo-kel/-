#include "controller.h";



void HandleActivity() {
	int x;
	std::cin >> x;
	ClientLogin(sid, pwd, clientPosition);//应在handleActivity中，由用户确认执行

	std::cin >> x;
	SendQuestion("they tested my DNA,it was not DNA but GPA");
}

int main() {

    //std::string file = ;
    /*LPCWSTR str = L"bitmap.bmp";
	HBITMAP hbitmap = ScreenShot();
    CreateBMPFile(L"bitmap.bmp", hbitmap);
    return 0;*/
	ClientInit();


	if (SocketConnection) {//创建新线程处理请求
		std::thread t(HandleRequest);
		t.detach();
		std::thread t1(HandleActivity);
		t1.detach();
	}

	int i = 1;
	while (1) {
		//HandleActivity();
		//SendBroadcastMessage("hello world"+std::to_string(i));
		Sleep(1000);
		i++;
	}
	return 0;
}