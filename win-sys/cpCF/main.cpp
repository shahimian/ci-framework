#include <windows.h>
#include <tchar.h>
#include <stdio.h>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int _tmain(int argc, LPTSTR argv[]) {
	if(argc != 3){
		printf("Usage: cpCF file count\n");
		return 1;
	}
	char buffer[10];
	for(int i = 1; i <= atoi(argv[2]); i++){
		sprintf(buffer, "%d", i);
		if(!CopyFile(argv[1], (LPCSTR)buffer, FALSE)){
			printf("CopyFile Error: %x\n", GetLastError());
			return 2;
		}
	}
	return 0;
}
