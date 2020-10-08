#include "controller.h";



void HandleActivity() {

}

int main() {

    //std::string file = ;
    /*LPCWSTR str = L"bitmap.bmp";
	HBITMAP hbitmap = ScreenShot();
    CreateBMPFile(L"bitmap.bmp", hbitmap);
    return 0;*/
	ClientInit();
	ClientLogin("123", "456", clientPosition);//应在handleActivity中，由用户确认执行
	if (SocketConnection) {
		std::thread t(HandleRequest);
		t.detach();
	}


	while (1) {
		HandleActivity();
		
		//SendBroadcastMessage("hello world");
		Sleep(1000);
	}
	return 0;
}