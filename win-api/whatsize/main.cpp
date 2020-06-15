#include <windows.h>

void Show(HWND hwnd, HDC hdc, int xText, int yText, int iMapMode, TCHAR *szMapMode){
	TCHAR szBuffer[60];
	RECT rect;
	SaveDC(hdc);
	SetMapMode(hdc, iMapMode);
	GetClientRect(hwnd, &rect);
	DPtoLP(hdc, (PPOINT)&rect, 2);
	RestoreDC (hdc, -1);
	TextOut(hdc, xText, yText, szBuffer, wsprintf (szBuffer, TEXT ("%-20s %7d %7d %7d %7d"), szMapMode, rect.left, rect.right, rect.top, rect.bottom));
	return;
}

/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	
	static TCHAR szHeading [] = TEXT ("Mapping Mode Left Right Top Bottom");
	static TCHAR szUndLine [] = TEXT ("------------ ---- ----- --- ------");
	
	HDC hdc;
	TEXTMETRIC tm;
	static int cxChar, cyChar;
	PAINTSTRUCT ps;
	
	switch(Message) {
		
		case WM_CREATE:
			hdc = GetDC(hwnd);
			SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));
			GetTextMetrics(hdc, &tm);
			cxChar = tm.tmAveCharWidth;
			cyChar = tm.tmHeight + tm.tmExternalLeading;
			ReleaseDC(hwnd, hdc);
			return 0;
			
		case WM_PAINT:
			hdc = BeginPaint(hwnd, &ps);
			SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));
			SetTextColor(hdc, 0x09f80c);
			SetBkColor(hdc, 0x0);
			SetMapMode(hdc, MM_ANISOTROPIC);
			SetWindowExtEx(hdc, 1, 1, NULL);
			SetViewportExtEx(hdc, cxChar, cyChar, NULL);
			
			TextOut(hdc, 1, 1, szHeading, lstrlen(szHeading));
			TextOut(hdc, 1, 2, szUndLine, lstrlen(szUndLine));
			
			Show (hwnd, hdc, 1, 3, MM_TEXT, TEXT ("TEXT (pixels)"));
 			Show (hwnd, hdc, 1, 4, MM_LOMETRIC, TEXT ("LOMETRIC (.1 mm)"));
 			Show (hwnd, hdc, 1, 5, MM_HIMETRIC, TEXT ("HIMETRIC (.01 mm)"));
 			Show (hwnd, hdc, 1, 6, MM_LOENGLISH, TEXT ("LOENGLISH (.01 in)"));
 			Show (hwnd, hdc, 1, 7, MM_HIENGLISH, TEXT ("HIENGLISH (.001 in)"));
 			Show (hwnd, hdc, 1, 8, MM_TWIPS, TEXT ("TWIPS (1/1440 in)"));
			
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
	
	static TCHAR szAppName[] = TEXT("WhatSize");

	/* zero out the struct and set the stuff we want to modify */
	memset(&wc,0,sizeof(wc));
	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.lpfnWndProc	 = WndProc; /* This is where we will send messages to */
	wc.hInstance	 = hInstance;
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	
	/* White, COLOR_WINDOW is just a #define for a system color, try Ctrl+Clicking it */
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszClassName = szAppName;
	wc.hIcon		 = LoadIcon(NULL, IDI_APPLICATION); /* Load a standard icon */
	wc.hIconSm		 = LoadIcon(NULL, IDI_APPLICATION); /* use the name "A" to use the project icon */

	if(!RegisterClassEx(&wc)) {
		MessageBox(NULL, "Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE,szAppName,TEXT("What Size is the Window?"),WS_VISIBLE|WS_OVERLAPPEDWINDOW,
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
