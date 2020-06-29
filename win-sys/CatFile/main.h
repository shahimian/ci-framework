#include <windows.h>
#include <tchar.h>

#define BUFFER_SIZE 0x200

static VOID CatFile(HANDLE, HANDLE);
int Options(int, LPTSTR, LPTSTR, BOOL);

static VOID CatFile(HANDLE hInFile, HANDLE hOutFile){
	DWORD nIn, nOut;
	BYTE buffer[BUFFER_SIZE];
	while(ReadFile(hInFile, buffer, BUFFER_SIZE, &nIn, NULL)
		&& (nIn != 0)
		&& WriteFile(hOutFile, buffer, BUFFER_SIZE, &nOut, NULL));
	return;
}

