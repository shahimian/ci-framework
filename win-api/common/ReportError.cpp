#include "common.h"

VOID ReportError(LPCSTR userMessage, DWORD exitCode, BOOL printErrorMessage){
	
	DWORD eMsgLen, errNum = GetLastError();
	LPTSTR lpvSysMsg;
	_ftprintf(stderr, _T("%s\n"), userMessage);
	
	if(printErrorMessage){
		eMsgLen = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM, NULL,
			errNum, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSRT)&lpvSysMsg, 0, NULL);
		if(eMsgLen > 0){
			_ftprintf(strerr, "%s\n", lpvSysMsg);
		} else {
			_ftprintf(strerr, _T("Last Error Number: %d.\n"), errNum);
		}
		if(lpvSysMsg != NULL) LocalFree(lpvSysMsg);
	}
	
	if(exitCode > 0)
		ExitProcess(exitCode);
	return;
}

