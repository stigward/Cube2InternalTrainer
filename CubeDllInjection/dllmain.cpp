// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include <iostream>
#include <vector>
#include <Windows.h>
#include <TlHelp32.h>
#include "proc.h"



DWORD WINAPI InjectionThread(HMODULE hmod) 
{
	AllocConsole();
	FILE *f;

	freopen_s(&f, "CONOUT$", "w", stdout);
	uintptr_t modBase = (uintptr_t)GetModuleHandle(L"sauerbraten.exe");
	
	bool godMode = true;
	std::cout << "In the injection thread\n";

	while (true)
	{
			// get address
		uintptr_t pointer = modBase + 0x29D2C0;
		if (godMode)
		{
			uintptr_t addr = FindDMAAddr(pointer, { 0x0, 0x340 });
			// create a pointer to the address we just found
			int* health = (int*)addr;
			//deref that pointer and fill health back up	
			*health = 9999;
			Sleep(1);	
		}

	}

	fclose(f);
	FreeConsole();
	FreeLibraryAndExitThread(hmod, 0);
	return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)InjectionThread, hModule, 0, nullptr));
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

