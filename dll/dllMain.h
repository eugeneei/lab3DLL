#pragma once
#ifdef DLLMAIN_EXPORTS
#define DLLMAIN_API __declspec(dllexport)
#else
#define DLLMAIN_API __declspec(dllimport)
#endif

extern "C" DLLMAIN_API void changeString(int pid, const char* initial, const char* replace);