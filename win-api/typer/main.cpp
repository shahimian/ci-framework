#include <windows.h>

#define BUFFER(x,y) *(pBuffer + y * cxBuffer + x)

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))

/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	
	static DWORD dwCharSet = DEFAULT_CHARSET;
	static int cxChar, cyChar, cxClient, cyClient, cxBuffer, cyBuffer, xCaret, yCaret;
	static TCHAR * pBuffer = NULL;
	HDC hdc;
	int x, y, i;
	PAINTSTRUCT ps;
	TEXTMETRIC tm;
	
	switch(Message) {

		case WM_INPUTLANGCHANGE:
			dwCharSet = wParam ;
			// fall through
		case WM_CREATE:
			hdc = GetDC (hwnd) ;
			SelectObject (hdc, CreateFont (0, 0, 0, 0, 0, 0, 0, 0, dwCharSet, 0, 0, 0, FIXED_PITCH, NULL)) ;
			GetTextMetrics (hdc, &tm) ;
			cxChar = tm.tmAveCharWidth ;
			cyChar = tm.tmHeight ;
			DeleteObject (SelectObject (hdc, GetStockObject (SYSTEM_FONT))) ;
			ReleaseDC (hwnd, hdc) ;
			// fall through
		case WM_SIZE:
			// obtain window size in pixels
			if (Message == WM_SIZE)
			{
				cxClient = LOWORD (lParam) ;
				cyClient = HIWORD (lParam) ;
			}
			// calculate window size in characters
			cxBuffer = max (1, cxClient / cxChar) ;
			cyBuffer = max (1, cyClient / cyChar) ;
			// allocate memory for buffer and clear it
			if (pBuffer != NULL)
				free (pBuffer) ;
			pBuffer = (TCHAR *) malloc (cxBuffer * cyBuffer * sizeof (TCHAR)) ;
			for (y = 0 ; y < cyBuffer ; y++)
				for (x = 0 ; x < cxBuffer ; x++)
					BUFFER(x,y) = ' ' ;
			// set caret to upper left corner
			xCaret = 0 ;
			yCaret = 0 ;
			if (hwnd == GetFocus ())
				SetCaretPos (xCaret * cxChar, yCaret * cyChar) ;
			InvalidateRect (hwnd, NULL, TRUE) ;
			return 0 ;
		case WM_SETFOCUS:
			// create and show the caret
			CreateCaret (hwnd, NULL, cxChar, cyChar) ;
			SetCaretPos (xCaret * cxChar, yCaret * cyChar) ;
			ShowCaret (hwnd) ;
			return 0 ;
		case WM_KILLFOCUS:
			// hide and destroy the caret
			HideCaret (hwnd) ;
			DestroyCaret () ;
			return 0 ;
		case WM_KEYDOWN:
			switch (wParam)
			{
				case VK_HOME: xCaret = 0 ; break ;
				case VK_END: xCaret = cxBuffer - 1 ; break ;
				case VK_PRIOR: yCaret = 0 ;	break ;
				case VK_NEXT: yCaret = cyBuffer - 1 ; break ;
				case VK_LEFT: xCaret = max (xCaret - 1, 0) ; break ;
				case VK_RIGHT: xCaret = min (xCaret + 1, cxBuffer - 1) ; break ;
				case VK_UP: yCaret = max (yCaret - 1, 0) ; break ;
				case VK_DOWN: yCaret = min (yCaret + 1, cyBuffer - 1) ; break ;
				case VK_DELETE:
					for (x = xCaret ; x < cxBuffer - 1 ; x++)
						BUFFER (x, yCaret) = BUFFER (x + 1, yCaret) ;
					BUFFER (cxBuffer - 1, yCaret) = ' ' ;
					HideCaret (hwnd) ;
					hdc = GetDC (hwnd) ;
					SelectObject (hdc, CreateFont (0, 0, 0, 0, 0, 0, 0, 0, dwCharSet, 0, 0, 0, FIXED_PITCH, NULL)) ;
				TextOut (hdc, xCaret * cxChar, yCaret * cyChar,	& BUFFER (xCaret, yCaret),cxBuffer - xCaret) ;
				DeleteObject (SelectObject (hdc, GetStockObject (SYSTEM_FONT))) ;
				ReleaseDC (hwnd, hdc) ;
				ShowCaret (hwnd) ;
				break ;
			}
			SetCaretPos (xCaret * cxChar, yCaret * cyChar) ;
			return 0 ;
		case WM_CHAR:
			for (i = 0 ; i < (int) LOWORD (lParam) ; i++)
			{
				switch (wParam)
				{
					case '\b': // backspace
					if (xCaret > 0)
					{
						xCaret-- ;
						SendMessage (hwnd, WM_KEYDOWN, VK_DELETE, 1) ;
					}
					break ;
					case '\t': // tab
						do
						{
							SendMessage (hwnd, WM_CHAR, ' ', 1) ;
						}
						while (xCaret % 8 != 0) ;
						break ;
						case '\n': // line feed
						if (++yCaret == cyBuffer)
							yCaret = 0 ;
						break ;
						case '\r': // carriage return
							xCaret = 0 ;
						if (++yCaret == cyBuffer)
							yCaret = 0 ;
						break ;
						case '\x1B': // escape
							for (y = 0 ; y < cyBuffer ; y++)
								for (x = 0 ; x < cxBuffer ; x++)
									BUFFER (x, y) = ' ' ;
							xCaret = 0 ;
							yCaret = 0 ;
							InvalidateRect (hwnd, NULL, FALSE) ;
					break ;
					default: // character codes
						BUFFER (xCaret, yCaret) = (TCHAR) wParam ;
						HideCaret (hwnd) ;
						hdc = GetDC (hwnd) ;
						SelectObject (hdc, CreateFont (0, 0, 0, 0, 0, 0, 0, 0,
						dwCharSet, 0, 0, 0, FIXED_PITCH, NULL)) ;
						TextOut (hdc, xCaret * cxChar, yCaret * cyChar, & BUFFER (xCaret, yCaret), 1) ;
						DeleteObject (SelectObject (hdc, GetStockObject (SYSTEM_FONT))) ;
						ReleaseDC (hwnd, hdc) ;
						ShowCaret (hwnd) ;
						if (++xCaret == cxBuffer)
						{
							xCaret = 0 ;
							if (++yCaret == cyBuffer)
							yCaret = 0 ;
						}
					break ;
				}
			}
			SetCaretPos (xCaret * cxChar, yCaret * cyChar) ;
			return 0 ;
		case WM_PAINT:
			hdc = BeginPaint (hwnd, &ps) ;
			SelectObject (hdc, CreateFont (0, 0, 0, 0, 0, 0, 0, 0, dwCharSet, 0, 0, 0, FIXED_PITCH, NULL)) ;
			for (y = 0 ; y < cyBuffer ; y++)
				TextOut (hdc, 0, y * cyChar, & BUFFER(0,y), cxBuffer) ;
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
	
	TCHAR szClassName[] = "TYPER";
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

	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE,szClassName,"Typer Program",WS_VISIBLE|WS_OVERLAPPEDWINDOW,
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

