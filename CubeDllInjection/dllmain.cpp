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
	bool maxAmmo = true;
	std::cout << "In the injection thread\n";

	while (true)
	{
		// get address
		uintptr_t pointer = modBase + 0x29D2C0;
		uintptr_t visual = modBase + 0x2074A4;
		int healthVal = 9999;
		if (godMode)
		{
			uintptr_t addr = FindDMAAddr(pointer, { 0x0, 0x340 });
			// create a pointer to the address we just found
			int* health = (int*)addr;
			//deref that pointer and fill health back up	
			*health = healthVal;

			// same thing but for the UI. Otherwise it would look like we have 100 instead of 9999
			uintptr_t v_addr = FindDMAAddr(visual, { 0x15c });
			int *v_health = (int*)v_addr;
			*v_health = healthVal;
			Sleep(1);	
		}

		if (maxAmmo) {
			uintptr_t ptr = modBase + 0x2074A4;
			uintptr_t ammoAddr = FindDMAAddr(ptr , { 0x17c });
			// total number of guns in the game
			int numGuns = 6;
			int ammoVal = 9999;
			for (unsigned int i = 0; i <= numGuns; ++i)
			{
				uintptr_t currentAddr = ammoAddr + i*4;
				int *ammo = (int*)currentAddr;
				*ammo = ammoVal;
				
			}

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

