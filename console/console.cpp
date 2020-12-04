// console.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include "windows.h"
#include "..\\dll\\dllMain.h"
//описание типа указателя на функцию
typedef void (*Func)();

void dynamicDLL(int pid) {
	HMODULE hMod = LoadLibraryA("C:\\Users\\evgen\\source\\repos\\lab3\\dll\\x64\\Debug\\dll.dll");
	if (hMod != NULL) {
		Func function = (Func)GetProcAddress(hMod, "changeString");
		if (function != NULL) {
			//function(pid);
		}
		FreeLibrary(hMod);
	}
}

void staticDLL(int pid) {
	//changeString(pid, "bsuir", "lab3");
}

void integrationDLL(int pid) {
	HMODULE hKernel = LoadLibraryA("kernel32.dll"); //статическая библиотека
	LPVOID loadLibrary = (LPVOID)GetProcAddress(hKernel, "LoadLibraryA"); //адрес функции
	HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, //права доступа
		false, //не наследуется от другого процесса
		pid    //id процесса, к которому внедряемся
	);
	if (handle) {
		//выделить память в другом процессе
		LPVOID memALLoc = VirtualAllocEx(handle, 
			nullptr, //адрес выделения памяти
			strlen("C:\\Users\\evgen\\source\\repos\\lab3\\dll\\x64\\Debug\\dll.dll") + 1, //размер выделяемой памяти
			MEM_RESERVE | MEM_COMMIT, //распределение расходов памяти
            PAGE_READWRITE); //защита страниц
		WriteProcessMemory(handle,
			memALLoc,
			"C:\\Users\\evgen\\source\\repos\\lab3\\dll\\x64\\Debug\\dll.dll",
			strlen("C:\\Users\\evgen\\source\\repos\\lab3\\dll\\x64\\Debug\\dll.dll") + 1,
			nullptr); //указатель на переменную, получающей кол-во байтов
		HANDLE hThread = CreateRemoteThread(handle,
			nullptr, //без атрибутов секьюрити
			0,
			(LPTHREAD_START_ROUTINE)loadLibrary,
			memALLoc,
			0,
			NULL);
		WaitForSingleObject(hThread, INFINITE); //ожидание завершения потока
		CloseHandle(hThread);
		CloseHandle(handle);
		FreeLibrary(hKernel);
	}
}

int main()
{
	int pid = 23028;
	//integrationDLL(pid);
	dynamicDLL(pid);
	//staticDLL(pid);
}
