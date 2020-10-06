#include "controller.h";



void HandleActivity() {

}

int main() {
	ClientInit();
	ClientLogin("123", "456", clientPosition);//应在handleActivity中，由用户确认执行
	while (1) {
		HandleActivity();
		Sleep(1000);
	}
}