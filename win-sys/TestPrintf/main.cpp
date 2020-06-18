#include <windows.h>
#include <tchar.h>
#include <iostream>
#include <cstdio>
#include <cstring>

#define VER "1.0.0"

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int _tmain(int argc, LPTSTR argv[]) {
	printf("printf/scanf Test %s\n", VER);
	printf("What's your name? ");
	TCHAR fullname[80];
	scanf("%79s", fullname);
	printf("\nHello %s! :)", fullname);
	return 0;
}

