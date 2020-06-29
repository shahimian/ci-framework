#include <windows.h>
#include "sysmets.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX wc; /* A properties struct of our window */
	HWND hwnd; /* A 'HANDLE', hence the H, or a pointer to our window */
	MSG msg; /* A temporary location for all messages */
	static TCHAR szAppName[] = TEXT("SysMets1");

	/* zero out the struct and set the stuff we want to modify */
	memset(&wc,0,sizeof(wc));
	wc.style		 = CS_HREDRAW | CS_VREDRAW;
	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.lpfnWndProc	 = WndProc; /* This is where we will send messages to */
	wc.hInstance	 = hInstance;
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	
	/* White, COLOR_WINDOW is just a #define for a system color, try Ctrl+Clicking it */
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszClassName = szAppName;
	wc.hIcon		 = LoadIcon(NULL, IDI_APPLICATION); /* Load a standard icon */
	wc.hIconSm		 = LoadIcon(NULL, IDI_APPLICATION); /* use the name "A" to use the project icon */

	if(!RegisterClassEx(&wc)) {
		MessageBox(NULL, TEXT("Window Registration Failed!"), TEXT("Error!"),MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE,szAppName,TEXT("Get System Metrics No. 1"),WS_VISIBLE|WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, /* x */
		CW_USEDEFAULT, /* y */
		640, /* width */
		480, /* height */
		NULL,NULL,hInstance,NULL);

	if(hwnd == NULL) {
		MessageBox(NULL, TEXT("Window Creation Failed!"), TEXT("Error!"),MB_ICONEXCLAMATION|MB_OK);
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

LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	static int	cxChar, cyChar, cxCaps, iVscrollPos, cyClient;
	TCHAR		szBuffer[10];
	int			i = 0, y;
	TEXTMETRIC	tm;
	HDC			hdc;
	PAINTSTRUCT	ps;
	switch(Message) {
		case WM_CREATE:
			hdc = GetDC(hwnd);
			GetTextMetrics(hdc, &tm);
			cxChar = tm.tmAveCharWidth;
			cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2) * cxChar / 2;
			cyChar = tm.tmHeight + tm.tmExternalLeading;
			ReleaseDC(hwnd, hdc);
			SetScrollRange(hwnd, SB_VERT, 0, NUMLINES - 1, FALSE);
			SetScrollPos(hwnd, SB_VERT, iVscrollPos, TRUE);
			return 0;
		case WM_SIZE:
			cyClient = HIWORD(wParam);
			return 0;
		case WM_VSCROLL:
			switch(LOWORD(wParam)){
				case SB_LINEUP:
					iVscrollPos -= 1;
					break;
				case SB_LINEDOWN:
					iVscrollPos += 1;
					break;
				case SB_PAGEUP:
					iVscrollPos -= cyClient / cyChar;
					break;
				case SB_PAGEDOWN:
					iVscrollPos += cyClient / cyChar;
					break;
				case SB_THUMBPOSITION:
					iVscrollPos = HIWORD(wParam);
					break;
				default:
					break;
			}
			iVscrollPos = max(0, min(iVscrollPos, NUMLINES - 1));
			if(iVscrollPos != GetScrollPos(hwnd, SB_VERT)){
				SetScrollPos(hwnd, SB_VERT, iVscrollPos, TRUE);
				InvalidateRect(hwnd, NULL, TRUE);
			}
			return 0;
		case WM_PAINT:
			hdc = BeginPaint(hwnd, &ps);
			for(i = 0; i < NUMLINES; i++){
				y = cyChar * ( i - iVscrollPos );
				TextOut(hdc, 0, y, sysmetrics[i].szLabel, lstrlen(sysmetrics[i].szLabel));
				TextOut(hdc, 22 * cxCaps, y, sysmetrics[i].szDesc, lstrlen(sysmetrics[i].szDesc));
				SetTextAlign(hdc, TA_RIGHT | TA_TOP);
				TextOut(hdc, 22 * cxCaps + 40 * cxChar, y, szBuffer, wsprintf(szBuffer, TEXT("%5d"), GetSystemMetrics(sysmetrics[i].iIndex)));
				SetTextAlign(hdc, TA_LEFT | TA_TOP);
			}
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

