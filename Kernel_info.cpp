
#include <Windows.h>
#include <stdio.h>
#include <Psapi.h>
#include "Kernel_Info.h"
//#include "Kernel_callbacks.h"
#include <comdef.h>
Kernel_Info::Kernel_Info()
{
	const char KernelName[] = "ntoskrnl.exe";
	EnumDeviceDrivers(BaseAddresses, sizeof(BaseAddresses), &cbNeeded);
	Debug = FALSE;
	Kr_ntoskrn_addr = GetDriverBaseAddr(KernelName);
	if (Debug)
		printf("[+] Loading the 0x%.8x image into the process'es address space\n", Kr_ntoskrn_addr);

}

DWORDLONG Kernel_Info::Kr_Get_fun_addr(PWCHAR fun_name)
{
	HMODULE hKernel;
	MODULEINFO KernelInfo;
	const char KernelName[] = "ntoskrnl.exe";
	PVOID pFunction;
	DWORDLONG OrgKernelStart;
	_bstr_t b(fun_name);
	const char* fun_nameA = b;
	if(Debug)
		printf("[+] Loading the %s image into the process'es address space\n", KernelName);
	hKernel = LoadLibraryEx(KernelName, NULL, DONT_RESOLVE_DLL_REFERENCES);
	if (hKernel == NULL && Debug)
	{
		printf("[-] LoadLibraryEx failed. LastError: 0x%.8x\n", (UINT)GetLastError());
		
	}
	if (!GetModuleInformation(GetCurrentProcess(), hKernel, &KernelInfo, sizeof(KernelInfo)) && Debug)
	{
		printf("[-] GetModuleInformation failed. LastError: 0x%.8x\n", (UINT)GetLastError());
	}

	pFunction = (PVOID)GetProcAddress(hKernel, fun_nameA);
	if (pFunction == NULL && Debug)
	{
		printf("[-] GetProcAddress failed. LastError: 0x%.8x\n", (UINT)GetLastError());
		return -1;
	}
	OrgKernelStart = GetOriginalImageBase(hKernel);
	if (hKernel)
	{
		FreeLibrary(hKernel);
	}
	return (DWORDLONG)pFunction - OrgKernelStart + Kr_ntoskrn_addr;
}

DWORDLONG Kernel_Info::GetOriginalImageBase(PVOID Module)
{
	PIMAGE_DOS_HEADER DosHeader = (PIMAGE_DOS_HEADER)Module;
	PIMAGE_NT_HEADERS NtHeaders = (PIMAGE_NT_HEADERS)((DWORDLONG)Module + DosHeader->e_lfanew);
	return NtHeaders->OptionalHeader.ImageBase;
}

DWORDLONG Kernel_Info::GetDriverBaseAddr(const char* BaseName)
{

	/*
	// Get a list of all the drivers' Image Base Addresses
	if (!EnumDeviceDrivers(BaseAddresses, sizeof(BaseAddresses), &cbNeeded)) return 0;
	*/
	CHAR FileName[MAX_PATH];

	/* Go thru the entire list */
	for (int i = 0; i < (int)(cbNeeded / sizeof(LPVOID)); i++)
	{
		/* For each image base, retrieve the driver's name */
		GetDeviceDriverBaseNameA(BaseAddresses[i], FileName, sizeof(FileName));

		/* In case of the current module being kernel, return its base */
		if (!_stricmp(FileName, BaseName)) return (DWORDLONG)BaseAddresses[i];
	}

	/* Should never get here */
	return 0;
}