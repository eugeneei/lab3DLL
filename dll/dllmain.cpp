// dllmain.cpp : Определяет точку входа для приложения DLL.
#include "pch.h"
#include "dllMain.h"
#include <iostream>
#include <process.h>
#include <vector>

//возможность экспорта без использования def-файлов (файлы определения модуля)
__declspec(dllexport) void changeString(int pid, const char* initial, const char* replace) {
	HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, //права доступа
		false, //не наследуется от другого процесса
		pid    //id процесса, к которому внедряемся
	);
	if (handle) {
		SYSTEM_INFO si;
		GetSystemInfo(&si);
		MEMORY_BASIC_INFORMATION info; //информация о памяти
		char* memory = 0; //указатель на память
		std::vector<char> container;
		while (memory < si.lpMaximumApplicationAddress) { //по всему адресному пр-ву
			if (VirtualQueryEx(handle, //поиск страницы приложения через инф-цию об участке памяти
				memory, 
				&info, 
				sizeof(info)) == sizeof(info)) {
				if (info.State == MEM_COMMIT && info.AllocationProtect == PAGE_READWRITE) { //поиск где можно читать/записывать
					memory = (char*)info.BaseAddress; //базовый адрес процесса
					container.resize(info.RegionSize);
					SIZE_T bytesRead;

					if (ReadProcessMemory(handle, 
						memory,
						&container[0], //буфер с содержимым адр.пр-вом процесса
						info.RegionSize, //сколько читать из процесса
						&bytesRead)) { //сколько передано в буфер
						for (size_t i = 0; i < (bytesRead - sizeof(initial)); ++i) {
							if (memcmp(initial, &container[i], sizeof(initial)) == 0)
							{
								char* ref = (char*)memory + i; //адрес со смещением
								for (int j = 0; j < sizeof(replace); j++) {
									ref[j] = replace[j];
								}
								ref[sizeof(replace)] = 0;
							}
						}
					}
				}
				memory += info.RegionSize;
			}
		}
	}
}

BOOL APIENTRY DllMain( HMODULE hModule, //дескриптор модуля DLL
                       DWORD  ul_reason_for_call, //код причины вызова точки входа
                       LPVOID lpReserved //резервация
                     )
{
	//система запускает или завершает процесс или поток
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH: //динамически: lpReserved = NULL
		changeString(_getpid(), "bsuir", "lab3");
		break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH: //неудачная загрузка или успешная: lpReserved = NULL
    case DLL_PROCESS_DETACH:
		break;
    }
    return TRUE;
}

