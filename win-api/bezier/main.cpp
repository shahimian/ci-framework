#include <windows.h>
#include "header.h"

/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	static POINT apt[4];
	HDC hdc;
	PAINTSTRUCT ps;
	int cxClient, cyClient;
	
	switch(Message) {
		
		case WM_SIZE:
			
			cxClient = LOWORD(lParam);
			cyClient = HIWORD(lParam);
			
			apt[0].x = cxClient / 4;
			apt[0].y = cyClient / 2;
			
			apt[1].x = cxClient / 2;
			apt[1].y = cyClient / 4;
			
			apt[2].x = cxClient / 2;
			apt[2].y = 3 * cyClient / 4;
			
			apt[3].x = 3 * cxClient / 4;
			apt[3].y = cyClient / 2;
			
			return 0;
			
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MOUSEMOVE:
			if(wParam & MK_LBUTTON || wParam & MK_RBUTTON){
				hdc = GetDC(hwnd);
				SelectObject(hdc, GetStockObject(WHITE_PEN));
				DrawBezier(hdc, apt);
				
				if(wParam & MK_LBUTTON){
					apt[1].x = LOWORD(lParam);
					apt[1].y = HIWORD(lParam);
				}
				
				if(wParam & MK_RBUTTON){
					apt[2].x = LOWORD(lParam);
					apt[2].y = HIWORD(lParam);
				}
				
				SelectObject(hdc, GetStockObject(BLACK_PEN));
				DrawBezier(hdc, apt);
				EndPaint(hwnd, &ps);
				
			}
			
		case WM_PAINT:
			InvalidateRect(hwnd, NULL, TRUE);
			hdc = BeginPaint(hwnd, &ps);
			DrawBezier(hdc, apt);
			EndPaint(hwnd, &ps);
			return 0;
		
		/* Upon destruction, tell the main thread to stop */
		case WM_DESTROY: {
			PostQuitMessage(0);
			break;
		}
		
		/* All other messages (a lot of them) are processed using default procedures */
		default:
			return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}

/* The 'main' function of Win32 GUI programs: this is where execution starts */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX wc; /* A properties struct of our window */
	HWND hwnd; /* A 'HANDLE', hence the H, or a pointer to our window */
	MSG msg; /* A temporary location for all messages */

	/* zero out the struct and set the stuff we want to modify */
	memset(&wc,0,sizeof(wc));
	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.lpfnWndProc	 = WndProc; /* This is where we will send messages to */
	wc.hInstance	 = hInstance;
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	
	/* White, COLOR_WINDOW is just a #define for a system color, try Ctrl+Clicking it */
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszClassName = "WindowClass";
	wc.hIcon		 = LoadIcon(NULL, IDI_APPLICATION); /* Load a standard icon */
	wc.hIconSm		 = LoadIcon(NULL, IDI_APPLICATION); /* use the name "A" to use the project icon */

	if(!RegisterClassEx(&wc)) {
		MessageBox(NULL, "Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE,"WindowClass",TEXT("Bezier"),WS_VISIBLE|WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, /* x */
		CW_USEDEFAULT, /* y */
		640, /* width */
		480, /* height */
		NULL,NULL,hInstance,NULL);

	if(hwnd == NULL) {
		MessageBox(NULL, "Window Creation Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	/*
		This is the heart of our program where all input is processed and 
		sent to WndProc. Note that GetMessage blocks code flow until it receives something, so
		this loop will not produce unreasonably high CPU usage
	*/
	while(GetMessage(&msg, NULL, 0, 0) > 0) { /* If no error is received... */
		TranslateMessage(&msg); /* Translate key codes to chars if present */
		DispatchMessage(&msg); /* Send it to WndProc */
	}
	return msg.wParam;
}

void DrawBezier(HDC hdc, POINT apt[])
{
	PolyBezier(hdc, apt, 4);
	
	MoveToEx(hdc, apt[0].x, apt[0].y, NULL);
	LineTo(hdc, apt[1].x, apt[1].y);
	
	MoveToEx(hdc, apt[2].x, apt[2].y, NULL);
	LineTo(hdc, apt[3].x, apt[3].y);
	
	return;
}
