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
	
	bool godMode = false;
	bool maxAmmo = false;
	std::cout << "Cheat Menu:\n[J]: Toggles Max Ammo\n[K] Toggles God Mode\n";
	std::cout << "All Cheats off...\n";

	while (true)
	{
		// get address


		if (GetKeyState(0x4a) & 0x8000)
		{
			maxAmmo = !maxAmmo;
			std::cout << "Toggled Max ammo: ";
			if (maxAmmo) {
				std::cout << "on\n";
				// pointer static address that holds gun
				uintptr_t pointer = modBase + 0x2074A4;

				// The layout of memory here is odd. Starting at offset 0x17c, we have our first gun
				// from there, there are 6 total guns. We need to fill each of these 6 addresses to get ammo in each possible weapon
				// This is the first gun in inventory
				uintptr_t ammoAddr = FindDMAAddr(pointer, { 0x17c });
				// total number of guns in the game
				int numGuns = 6;
				//value to set ammo to
				int ammoVal = 9999;

				// iterate through each of the ammo spaces in memory
				for (unsigned int i = 0; i <= numGuns; ++i)
				{
					// multiply our index by 4 to create the offset from ammoAddr and then add them together
					uintptr_t currentAddr = ammoAddr + i * 4;
					
					//Create ptr, deref, set equal to ammoVal
					int *ammo = (int*)currentAddr;
					*ammo = ammoVal;
				}

			}
			else 
			{
				std::cout << "off\n";
			}
			Sleep(500);
		}

		if (GetKeyState(0x4b) & 0x8000)
		{
			godMode = !godMode;
			std::cout << "Toggled God Mode: ";
			if (godMode)
			{
				std::cout << "on\n";
				// pointer to the entity class
				uintptr_t pointer = modBase + 0x29D2C0;
				// pointer to the gui
				uintptr_t visual_pointer = modBase + 0x2074A4;

				// value we will replace our health with
				int healthVal = 9999;

				// get the health addr from the pointer
				uintptr_t addr = FindDMAAddr(pointer, { 0x0, 0x340 });
				// create a pointer to the address we just found
				int* health = (int*)addr;
				//deref that pointer and and fill with health
				*health = healthVal;

				// same thing but for the UI. Otherwise on the injection, our health would still show 100 until we were hit
				// and the ui was written to by the entity
				uintptr_t v_addr = FindDMAAddr(visual_pointer, { 0x15c });
				int *v_health = (int*)v_addr;
				*v_health = healthVal;
				}
			else
			{
				std::cout << "off\n";
			}
			Sleep(500);
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

