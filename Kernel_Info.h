#pragma once

class Kernel_Info{
private:
	
	DWORDLONG Kr_ntoskrn_addr;
	LPVOID BaseAddresses[4096];
	DWORD cbNeeded;
	BOOL Debug;

public:
	Kernel_Info();
	DWORDLONG Kr_Get_fun_addr(PWCHAR);
	DWORDLONG GetOriginalImageBase(PVOID Module);
	DWORDLONG GetDriverBaseAddr(const char* BaseName);
};