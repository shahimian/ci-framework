#include <windows.h>
#include <tchar.h>

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))

/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {

	static struct
	{
		int idStockFont ;
		TCHAR *szStockFont;
	}
	stockfont[] = {OEM_FIXED_FONT, "OEM_FIXED_FONT",
		ANSI_FIXED_FONT, "ANSI_FIXED_FONT",
		ANSI_VAR_FONT, "ANSI_VAR_FONT",
		SYSTEM_FONT, "SYSTEM_FONT",
		DEVICE_DEFAULT_FONT, "DEVICE_DEFAULT_FONT",
		SYSTEM_FIXED_FONT, "SYSTEM_FIXED_FONT",
		DEFAULT_GUI_FONT, "DEFAULT_GUI_FONT"};
		
	static int iFont, cFonts = sizeof stockfont / sizeof stockfont[0];
	HDC hdc;
	int i, x, y, cxGrid, cyGrid;
	PAINTSTRUCT ps;
	TCHAR szFaceName [LF_FACESIZE], szBuffer[LF_FACESIZE + 64];
	TEXTMETRIC tm;
	
	switch(Message) {
		
		case WM_CREATE:
			SetScrollRange(hwnd, SB_VERT, 0, cFonts - 1, TRUE);
			return 0;
			
		case WM_DISPLAYCHANGE:
			InvalidateRect(hwnd, NULL, TRUE);
			return 0;
			
		case WM_VSCROLL:
			switch (LOWORD (wParam))
			{
				case SB_TOP: iFont = 0; break;
				case SB_BOTTOM: iFont = cFonts - 1; break;
				case SB_LINEUP:
				case SB_PAGEUP: iFont -= 1; break;
				case SB_LINEDOWN:
				case SB_PAGEDOWN: iFont += 1; break;
				case SB_THUMBPOSITION: iFont = HIWORD (wParam); break;
			}
			iFont = max(0, min(cFonts - 1, iFont));
			SetScrollPos (hwnd, SB_VERT, iFont, TRUE);
			InvalidateRect (hwnd, NULL, TRUE);
			return 0 ;
		case WM_KEYDOWN:
			switch (wParam)
			{
				case VK_HOME: SendMessage (hwnd, WM_VSCROLL, SB_TOP, 0); break;
				case VK_END: SendMessage (hwnd, WM_VSCROLL, SB_BOTTOM, 0); break;
				case VK_PRIOR:
				case VK_LEFT:
				case VK_UP: SendMessage (hwnd, WM_VSCROLL, SB_LINEUP, 0); break;
				case VK_NEXT:
				case VK_RIGHT:
				case VK_DOWN: SendMessage (hwnd, WM_VSCROLL, SB_PAGEDOWN, 0); break;
			}
		return 0;
		
		case WM_PAINT:
			hdc = BeginPaint (hwnd, &ps);
			SelectObject (hdc, GetStockObject (stockfont[iFont].idStockFont));
			GetTextFace (hdc, LF_FACESIZE, szFaceName);
			GetTextMetrics (hdc, &tm);
			cxGrid = max (3 * tm.tmAveCharWidth, 2 * tm.tmMaxCharWidth);
			cyGrid = tm.tmHeight + 3;
			TextOut (hdc, 0, 0, szBuffer, wsprintf (szBuffer, TEXT (" %s: Face Name = %s, CharSet = %i"), stockfont[iFont].szStockFont, szFaceName, tm.tmCharSet));
			SetTextAlign (hdc, TA_TOP | TA_CENTER);
			// vertical and horizontal lines
			for (i = 0 ; i < 17 ; i++)
			{
				MoveToEx (hdc, (i + 2) * cxGrid, 2 * cyGrid, NULL);
				LineTo (hdc, (i + 2) * cxGrid, 19 * cyGrid);
				MoveToEx (hdc, cxGrid, (i + 3) * cyGrid, NULL);
				LineTo (hdc, 18 * cxGrid, (i + 3) * cyGrid);
			}
			// vertical and horizontal headings
			for (i = 0 ; i < 16 ; i++)
			{
				TextOut (hdc, (2 * i + 5) * cxGrid / 2, 2 * cyGrid + 2, szBuffer, wsprintf (szBuffer, TEXT ("%X-"), i));
				TextOut (hdc, 3 * cxGrid / 2, (i + 3) * cyGrid + 2, szBuffer, wsprintf (szBuffer, TEXT ("-%X"), i));
			}
			// characters
			for (y = 0 ; y < 16 ; y++)
				for (x = 0 ; x < 16 ; x++)
				{
					TextOut (hdc, (2 * x + 5) * cxGrid / 2, (y + 3) * cyGrid + 2, szBuffer, wsprintf (szBuffer, TEXT ("%c"), 16 * x + y));
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
	WNDCLASSEX wc; /* A properties struct of our window */
	HWND hwnd; /* A 'HANDLE', hence the H, or a pointer to our window */
	MSG msg; /* A temporary location for all messages */
	
	static TCHAR szClassName[] = _T("STOKFONT");

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

	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE,szClassName,_T("Stock Font Object"),WS_VISIBLE|WS_OVERLAPPEDWINDOW,
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
