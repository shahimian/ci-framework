#include <windows.h>
#include <math.h>

#define TWO_PI (2 * 3.14159)


/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	
	static int cxClient, cyClient;
	HCURSOR hCursor;
	static HRGN hRgnClip;
	HRGN hRgnTemp[6];
	int i;
	HDC hdc;
	PAINTSTRUCT ps;
	double fRadius, fAngle;
	
	switch(Message) {
		
		case WM_SIZE:
			cxClient = LOWORD(lParam);
			cyClient = LOWORD(lParam);
			
			hCursor = SetCursor(LoadCursor(NULL, IDC_CROSS));
			ShowCursor(TRUE);
			
			if(hRgnClip)
				DeleteObject(hRgnClip);
				
			hRgnTemp[0] = CreateEllipticRgn(0, cyClient / 3, cxClient / 2, 2 * cyClient / 3);
			hRgnTemp[1] = CreateEllipticRgn (cxClient / 2, cyClient / 3, cxClient, 2 * cyClient / 3);
 			hRgnTemp[2] = CreateEllipticRgn (cxClient / 3, 0, 2 * cxClient / 3, cyClient / 2);
 			hRgnTemp[3] = CreateEllipticRgn (cxClient / 3, cyClient / 2, 2 * cxClient / 3, cyClient);
 			hRgnTemp[4] = CreateRectRgn (0, 0, 1, 1);
 			hRgnTemp[5] = CreateRectRgn (0, 0, 1, 1);
 			hRgnClip = CreateRectRgn (0, 0, 1, 1);
 			CombineRgn (hRgnTemp[4], hRgnTemp[0], hRgnTemp[1], RGN_OR);
 			CombineRgn (hRgnTemp[5], hRgnTemp[2], hRgnTemp[3], RGN_OR);
 			CombineRgn (hRgnClip, hRgnTemp[4], hRgnTemp[5], RGN_XOR);
 			for (i = 0 ; i < 6 ; i++)
 				DeleteObject (hRgnTemp[i]) ;
 			SetCursor (hCursor) ;
 			ShowCursor (FALSE) ;
			
			return 0;
			
		case WM_PAINT:
			hdc = BeginPaint (hwnd, &ps);
			SetViewportOrgEx (hdc, cxClient / 2, cyClient / 2, NULL);
			SelectClipRgn (hdc, hRgnClip);
			fRadius = _hypot (cxClient / 2.0, cyClient / 2.0);
			for (fAngle = 0.0 ; fAngle < TWO_PI ; fAngle += TWO_PI / 360)
			{
				MoveToEx (hdc, 0, 0, NULL);
				LineTo (hdc, (int) ( fRadius * cos (fAngle) + 0.5), (int) (-fRadius * sin (fAngle) + 0.5));
			}
			EndPaint (hwnd, &ps);
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
	
	static TCHAR szAppName[] = "CLOVER";
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
	wc.lpszClassName = szAppName;
	wc.hIcon		 = LoadIcon(NULL, IDI_APPLICATION); /* Load a standard icon */
	wc.hIconSm		 = LoadIcon(NULL, IDI_APPLICATION); /* use the name "A" to use the project icon */

	if(!RegisterClassEx(&wc)) {
		MessageBox(NULL, "Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE,szAppName,TEXT("Draw a Clover"),WS_VISIBLE|WS_OVERLAPPEDWINDOW,
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

