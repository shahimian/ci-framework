#include <windows.h>
#define DIVISIONS 5

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))

/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {

	static BOOL fState[DIVISIONS][DIVISIONS];
	static int cxBlock, cyBlock;
	HDC hdc;
	int x, y;
	PAINTSTRUCT ps;
	RECT rect;
	POINT point;
	
	switch(Message) {
		
		case WM_SIZE :
			cxBlock = LOWORD (lParam) / DIVISIONS ;
			cyBlock = HIWORD (lParam) / DIVISIONS ;
			return 0 ;
			
		case WM_SETFOCUS :
			ShowCursor (TRUE) ;
			return 0 ;
		case WM_KILLFOCUS :
			ShowCursor (FALSE) ;
			return 0 ;
		case WM_KEYDOWN :
			GetCursorPos (&point) ;
			ScreenToClient (hwnd, &point) ;
			x = max (0, min (DIVISIONS - 1, point.x / cxBlock)) ;
			y = max (0, min (DIVISIONS - 1, point.y / cyBlock)) ;
			switch (wParam)
			{
				case VK_UP :
					y-- ;
					break ;
				case VK_DOWN :
					y++ ;
					break ;
				case VK_LEFT :
					x-- ;
					break ;
				case VK_RIGHT :
					x++ ;
					break ;
				case VK_HOME :
					x = y = 0 ;
					break ;
				case VK_END :
					x = y = DIVISIONS - 1 ;
					break ;
				case VK_RETURN :
				case VK_SPACE :
					SendMessage (hwnd, WM_LBUTTONDOWN, MK_LBUTTON,
					MAKELONG (x * cxBlock, y * cyBlock)) ;
				break ;
			}
			x = (x + DIVISIONS) % DIVISIONS ;
			y = (y + DIVISIONS) % DIVISIONS ;
			point.x = x * cxBlock + cxBlock / 2 ;
			point.y = y * cyBlock + cyBlock / 2 ;
			ClientToScreen (hwnd, &point) ;
			SetCursorPos (point.x, point.y) ;
			return 0 ;			
		
		case WM_LBUTTONDOWN :
			x = LOWORD (lParam) / cxBlock ;
			y = HIWORD (lParam) / cyBlock ;
			if (x < DIVISIONS && y < DIVISIONS)
			{
				fState [x][y] ^= 1 ;
				rect.left = x * cxBlock ;
				rect.top = y * cyBlock ;
				rect.right = (x + 1) * cxBlock ;
				rect.bottom = (y + 1) * cyBlock ;
				InvalidateRect (hwnd, &rect, FALSE) ;
			}
			else
				MessageBeep (0) ;
			return 0 ;
		case WM_PAINT :
			hdc = BeginPaint (hwnd, &ps) ;
			for (x = 0 ; x < DIVISIONS ; x++)
				for (y = 0 ; y < DIVISIONS ; y++)
				{
					Rectangle (hdc, x * cxBlock, y * cyBlock, (x + 1) * cxBlock, (y + 1) * cyBlock);
					if (fState [x][y])
					{
						MoveToEx (hdc, x * cxBlock, y * cyBlock, NULL) ;
						LineTo (hdc, (x+1) * cxBlock, (y+1) * cyBlock) ;
						MoveToEx (hdc, x * cxBlock, (y+1) * cyBlock, NULL) ;
						LineTo (hdc, (x+1) * cxBlock, y * cyBlock) ;
					}
				}
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
	TCHAR szClassName[] = "Checker";
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

	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE,szClassName,TEXT("Checker"),WS_VISIBLE|WS_OVERLAPPEDWINDOW,
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

