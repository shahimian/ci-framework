#include <windows.h>

#define NUMLINES ((int) (sizeof devcaps / sizeof devcaps [0]))

struct
{
 int iIndex;
 TCHAR * szLabel;
 TCHAR * szDesc;
}
devcaps [] = {
 HORZSIZE, TEXT ("HORZSIZE"), TEXT ("Width in millimeters:"),
 VERTSIZE, TEXT ("VERTSIZE"), TEXT ("Height in millimeters:"),
 HORZRES, TEXT ("HORZRES"), TEXT ("Width in pixels:"),
 VERTRES, TEXT ("VERTRES"), TEXT ("Height in raster lines:"),
 BITSPIXEL, TEXT ("BITSPIXEL"), TEXT ("Color bits per pixel:"),
 PLANES, TEXT ("PLANES"), TEXT ("Number of color planes:"),
 NUMBRUSHES, TEXT ("NUMBRUSHES"), TEXT ("Number of device brushes:"),
 NUMPENS, TEXT ("NUMPENS"), TEXT ("Number of device pens:"),
 NUMMARKERS, TEXT ("NUMMARKERS"), TEXT ("Number of device markers:"),
 NUMFONTS, TEXT ("NUMFONTS"), TEXT ("Number of device fonts:"),
 NUMCOLORS, TEXT ("NUMCOLORS"), TEXT ("Number of device colors:"),
 PDEVICESIZE, TEXT ("PDEVICESIZE"), TEXT ("Size of device structure:"),
 ASPECTX, TEXT ("ASPECTX"), TEXT ("Relative width of pixel:"),
 ASPECTY, TEXT ("ASPECTY"), TEXT ("Relative height of pixel:"),
 ASPECTXY, TEXT ("ASPECTXY"), TEXT ("Relative diagonal of pixel:"),
 LOGPIXELSX, TEXT ("LOGPIXELSX"), TEXT ("Horizontal dots per inch:"),
 LOGPIXELSY, TEXT ("LOGPIXELSY"), TEXT ("Vertical dots per inch:"),
 SIZEPALETTE, TEXT ("SIZEPALETTE"), TEXT ("Number of palette entries:"),
 NUMRESERVED, TEXT ("NUMRESERVED"), TEXT ("Reserved palette entries:"),
 COLORRES, TEXT ("COLORRES"), TEXT ("Actual color resolution:")
};

/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	static int cxChar, cxCaps, cyChar ;
	TCHAR szBuffer[10] ;
	HDC hdc ;
	int i ;
	PAINTSTRUCT ps ;
	TEXTMETRIC tm ;
	
	switch(Message) {
		
		case WM_CREATE:
			hdc = GetDC (hwnd) ;
				GetTextMetrics (hdc, &tm) ;
				cxChar = tm.tmAveCharWidth ;
				cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2) * cxChar / 2 ;
				cyChar = tm.tmHeight + tm.tmExternalLeading ;
			ReleaseDC (hwnd, hdc) ;
		return 0 ;
		
		case WM_PAINT:
			hdc = BeginPaint (hwnd, &ps) ;
				for (i = 0 ; i < NUMLINES ; i++)
				{
					TextOut (hdc, 0, cyChar * i,
					devcaps[i].szLabel,
					lstrlen (devcaps[i].szLabel)) ;
					TextOut (hdc, 14 * cxCaps, cyChar * i,
					devcaps[i].szDesc,
					lstrlen (devcaps[i].szDesc)) ;
					SetTextAlign (hdc, TA_RIGHT | TA_TOP) ;
					TextOut (hdc, 14 * cxCaps + 35 * cxChar, cyChar * i, szBuffer,
					wsprintf (szBuffer, TEXT ("%5d"),
					GetDeviceCaps (hdc, devcaps[i].iIndex))) ;
					SetTextAlign (hdc, TA_LEFT | TA_TOP) ;
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

	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE,"WindowClass","Caption",WS_VISIBLE|WS_OVERLAPPEDWINDOW,
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
