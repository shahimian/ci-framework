#include <windows.h>

#define min(a,b) ((a)<(b)?(a):(b))

/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	
	static DWORD dwCharSet = DEFAULT_CHARSET ;
	static int cxClientMax, cyClientMax, cxClient, cyClient, cxChar, cyChar ;
	static int cLinesMax, cLines ;
	static PMSG pmsg ;
	static RECT rectScroll ;
	static TCHAR szTop[] = TEXT ("Message Key Char ")
		TEXT ("Repeat Scan Ext ALT Prev Tran") ;
	static TCHAR szUnd[] = TEXT ("_______ ___ ____ ")
		TEXT ("______ ____ ___ ___ ____ ____") ;
	static TCHAR * szFormat[2] = {
		TEXT ("%-13s %3d %-15s%c%6u %4d %3s %3s %4s %4s"),
		TEXT ("%-13s 0x%04X%1s%c %6u %4d %3s %3s %4s %4s") } ;
	static TCHAR * szYes = TEXT ("Yes") ;
	static TCHAR * szNo = TEXT ("No") ;
	static TCHAR * szDown = TEXT ("Down") ;
	static TCHAR * szUp = TEXT ("Up") ;
	static TCHAR * szMessage [] = {
		TEXT ("WM_KEYDOWN"), TEXT ("WM_KEYUP"),
		TEXT ("WM_CHAR"), TEXT ("WM_DEADCHAR"),
		TEXT ("WM_SYSKEYDOWN"), TEXT ("WM_SYSKEYUP"),
		TEXT ("WM_SYSCHAR"), TEXT ("WM_SYSDEADCHAR") } ;
	HDC hdc ;
	int i, iType ;
	PAINTSTRUCT ps ;
	TCHAR szBuffer[128], szKeyName [32] ;
	TEXTMETRIC tm ;	
	
	switch(Message) {

		case WM_INPUTLANGCHANGE:
			dwCharSet = wParam ;
			// fall through
		case WM_CREATE:
		case WM_DISPLAYCHANGE:
			// Get maximum size of client area
			cxClientMax = GetSystemMetrics (SM_CXMAXIMIZED) ;
			cyClientMax = GetSystemMetrics (SM_CYMAXIMIZED) ;
			// Get character size for fixed-pitch font
			hdc = GetDC (hwnd) ;
			SelectObject (hdc, CreateFont (0, 0, 0, 0, 0, 0, 0, 0,
			dwCharSet, 0, 0, 0, FIXED_PITCH, NULL)) ;
			GetTextMetrics (hdc, &tm) ;
			cxChar = tm.tmAveCharWidth ;
			cyChar = tm.tmHeight ;
			DeleteObject (SelectObject (hdc, GetStockObject (SYSTEM_FONT))) ;
			ReleaseDC (hwnd, hdc) ;
			// Allocate memory for display lines
			if (pmsg)
				free (pmsg) ;
			cLinesMax = cyClientMax / cyChar ;
			pmsg = malloc(cLinesMax * sizeof(MSG));
			cLines = 0 ;
			// fall through
		case WM_SIZE:
			if (Message == WM_SIZE)
			{
				cxClient = LOWORD (lParam) ;
				cyClient = HIWORD (lParam) ;
			}
			// Calculate scrolling rectangle
			rectScroll.left = 0 ;
			rectScroll.right = cxClient ;
			rectScroll.top = cyChar ;
			rectScroll.bottom = cyChar * (cyClient / cyChar) ;
			InvalidateRect (hwnd, NULL, TRUE) ;
			if (Message == WM_INPUTLANGCHANGE)
				return TRUE ;
		return 0 ;
		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_CHAR:
		case WM_DEADCHAR:
		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
		case WM_SYSCHAR:
		case WM_SYSDEADCHAR:
			// Rearrange storage array
			for (i = cLinesMax - 1 ; i > 0 ; i--)
			{
				pmsg[i] = pmsg[i - 1] ;
			}
			// Store new message
			pmsg[0].hwnd = hwnd ;
			pmsg[0].message = Message ;
			pmsg[0].wParam = wParam ;
			pmsg[0].lParam = lParam ;
			cLines = min (cLines + 1, cLinesMax) ;
			// Scroll up the display
			ScrollWindow (hwnd, 0, -cyChar, &rectScroll, &rectScroll) ;
			break ; // ie, call DefWindowProc so Sys messages work
		case WM_PAINT:
			hdc = BeginPaint (hwnd, &ps) ;
			SelectObject (hdc, CreateFont (0, 0, 0, 0, 0, 0, 0, 0,dwCharSet, 0, 0, 0, FIXED_PITCH, NULL)) ;
			SetBkMode (hdc, TRANSPARENT) ;
			TextOut (hdc, 0, 0, szTop, lstrlen (szTop)) ;
			TextOut (hdc, 0, 0, szUnd, lstrlen (szUnd)) ;
			for (i = 0 ; i < min (cLines, cyClient / cyChar - 1) ; i++)
			{
				iType = pmsg[i].message == WM_CHAR ||
				pmsg[i].message == WM_SYSCHAR ||
				pmsg[i].message == WM_DEADCHAR ||
				pmsg[i].message == WM_SYSDEADCHAR ;
				GetKeyNameText (pmsg[i].lParam, szKeyName, sizeof (szKeyName) / sizeof (TCHAR)) ;
				TextOut (hdc, 0, (cyClient / cyChar - 1 - i) * cyChar, szBuffer,
				wsprintf (szBuffer, szFormat [iType],
				szMessage [pmsg[i].message
				- WM_KEYFIRST],
				pmsg[i].wParam,
				(PTSTR) (iType ? TEXT (" ") : szKeyName),
				(TCHAR) (iType ? pmsg[i].wParam : ' '),
				LOWORD (pmsg[i].lParam),
				HIWORD (pmsg[i].lParam) & 0xFF,
				0x01000000 & pmsg[i].lParam ? szYes : szNo,
				0x20000000 & pmsg[i].lParam ? szYes : szNo,
				0x40000000 & pmsg[i].lParam ? szDown : szUp,
				0x80000000 & pmsg[i].lParam ? szUp : szDown)) ;
			}
			DeleteObject (SelectObject (hdc, GetStockObject (SYSTEM_FONT))) ;
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
	
	static TCHAR szClassName[] = "KeyView2";
	
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

	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE,szClassName,"keyboard View Multi Language",WS_VISIBLE|WS_OVERLAPPEDWINDOW,
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
