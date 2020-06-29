#include <windows.h>
#include <tchar.h>

#define BUF_SIZE 0x200

static VOID CatFile(HANDLE, HANDLE);

int _tmain(int argc, LPTSTR argv[]) {
	HANDLE hInFile, hStdIn = GetStdHandle(STD_INPUT_HANDLE);
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CatFile(hStdIn, hStdOut);
	return 0;
}

static VOID CatFile(HANDLE hInFile, HANDLE hOutFile){
	DWORD nIn, nOut;
	BYTE buffer[BUF_SIZE];
	while(ReadFile(hInFile, buffer, BUF_SIZE, &nIn, NULL)
		&& (nIn != 0)
		&& WriteFile(hOutFile, buffer, nIn, &nOut, NULL));
	return;
}

