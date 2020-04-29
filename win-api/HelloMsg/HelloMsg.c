#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevinstance, PSTR szCmdLine, int iCmdShow){
	MessageBox(NULL, TEXT("Hello My World!"), TEXT("Hello My Friend!"), MB_OKCANCEL | MB_ICONHAND);
	return 0;
}
