#include <windows.h>
#include <tchar.h>
#include <stdio.h>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int _tmain(int argc, LPTSTR argv[]) {
	if(argc != 3){
		printf("Usage: cpCF file count\n");
		return 1;
	}
	if(!CopyFile(argv[1], argv[2], FALSE)){
		printf("CopyFile Error: %x\n", GetLastError());
		return 2;
	}
	return 0;
}
