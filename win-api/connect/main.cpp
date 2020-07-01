#include <windows.h>

#define MAXPOINTS 1000

/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	
	static POINT pt[MAXPOINTS] ;
	static int iCount ;
	HDC hdc ;
	int i, j ;
	PAINTSTRUCT ps ;
	
	switch(Message) {
		
		case WM_LBUTTONDOWN:
			iCount = 0 ;
			InvalidateRect (hwnd, NULL, TRUE) ;
			return 0 ;
		case WM_MOUSEMOVE:
			if (wParam & MK_LBUTTON && iCount < 1000)
			{
				pt[iCount ].x = LOWORD (lParam) ;
				pt[iCount++].y = HIWORD (lParam) ;
				hdc = GetDC (hwnd) ;
				SetPixel (hdc, LOWORD (lParam), HIWORD (lParam), 0) ;
				ReleaseDC (hwnd, hdc) ;
			}
			return 0 ;
		case WM_LBUTTONUP:
			InvalidateRect (hwnd, NULL, FALSE) ;
		return 0 ;
			case WM_PAINT:
			hdc = BeginPaint (hwnd, &ps) ;
			SetCursor (LoadCursor (NULL, IDC_WAIT)) ;
			ShowCursor (TRUE) ;
			for (i = 0 ; i < iCount - 1 ; i++)
				for (j = i + 1 ; j < iCount ; j++)
				{
					MoveToEx (hdc, pt[i].x, pt[i].y, NULL) ;
					LineTo (hdc, pt[j].x, pt[j].y) ;
				}
			ShowCursor (FALSE) ;
			SetCursor (LoadCursor (NULL, IDC_ARROW)) ;
			EndPaint (hwnd, &ps) ;
			return 0 ;
				
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
	TCHAR szClassName[] = "CONNECT";
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
	wc.lpszClassName = szClassName;
	wc.hIcon		 = LoadIcon(NULL, IDI_APPLICATION); /* Load a standard icon */
	wc.hIconSm		 = LoadIcon(NULL, IDI_APPLICATION); /* use the name "A" to use the project icon */

	if(!RegisterClassEx(&wc)) {
		MessageBox(NULL, "Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE,szClassName,"Connect P2P",WS_VISIBLE|WS_OVERLAPPEDWINDOW,
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
