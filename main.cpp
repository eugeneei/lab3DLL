#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "windows.h"
#include <process.h>

int main() {
	int pid = _getpid();
	char* string;
	string = (char*)calloc(256, sizeof(char));
	strcpy(string, "bsuir");
	std::cout << pid << std::endl;
	while (true) {
		Sleep(3000);
	    std::cout << string << std::endl;
	}
}