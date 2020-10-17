#include "Proc_Notifi.h"
#include <stdio.h>
#include <winternl.h>
#include <iostream>
#include "leechcore.h"
#include "vmmdll.h"
#include "SystemModules.h"
#pragma comment(lib, "vmm.lib")

static boolean Debug = FALSE;



int main()
{
	Debug = FALSE;
	BOOL result;
	//TCHAR buffer[MAX_PATH];
	LPCSTR param[] = { "","-device","pmem" };
	
	

	result = VMMDLL_Initialize(3, (LPSTR*)param);
	if (!result)
	{
		DWORD dw = GetLastError();
		if (Debug)
		{
			printf("[-] VMMDLL_Initialize: 0x%.8x\n", (UINT)GetLastError());
			//VMMDLL_Close();
			return 1;
		}
		if (dw == 0x6)
		{
			result = VMMDLL_Initialize(3, (LPSTR*)param);
			if (!result)
				return 1;
		}

	}

	notify_list_process(Process);
	notify_list_image(Image);
	notify_list_thread( Thread);
	notify_list_reg( Reg);
	VMMDLL_Close();
	return 0;
}

NTSTATUS notify_list_process (CallBackType CallType)
{
	return notify_list( ProcessReferences, ARRAYSIZE(ProcessReferences), (PUCHAR*)&PspCreateProcessNotifyRoutine, &PspCreateProcessNotifyRoutineMax, CallType);
}

NTSTATUS notify_list_image( CallBackType CallType)
{
	return notify_list( ImageReferences, ARRAYSIZE(ImageReferences), (PUCHAR*)&PspLoadImageNotifyRoutine, &PspLoadImageNotifyRoutineMax, CallType);
}

NTSTATUS notify_list_thread( CallBackType CallType)
{
	return notify_list( ThreadReferences, ARRAYSIZE(ThreadReferences), (PUCHAR*)&PspCreateThreadNotifyRoutine, &PspCreateThreadNotifyRoutineMax, CallType);
}

NTSTATUS notify_list_reg( CallBackType CallType)
{
	NTSTATUS status = STATUS_SUCCESS;
	PKKLL_M_NOTIFY_CALLBACK pNotifyCallback;
	PLIST_ENTRY pEntry = new _LIST_ENTRY;
	ULONG i;

	if (!CallbackListHeadOrCmpCallBackVector)
		status = notify_search(RegReferences, ARRAYSIZE(RegReferences), (PUCHAR*)&CallbackListHeadOrCmpCallBackVector, NULL, &pCmpCallBackOffsets);

	if (CallbackListHeadOrCmpCallBackVector)
	{
		
		bool Count =TRUE;
		DWORDLONG head = (DWORDLONG)CallbackListHeadOrCmpCallBackVector;
		if (VMMDLL_MemRead(4, head, (PBYTE)pEntry, sizeof(_LIST_ENTRY)))
		{
			SystemModules Sys_modules;
			while(Count)
			{
				DWORDLONG addr = 0;
				if (VMMDLL_MemRead(4, (DWORDLONG)((ULONG_PTR)head + pCmpCallBackOffsets->off1), (PBYTE)&addr, sizeof(DWORDLONG)))
				{
					if (!Sys_modules.PrintModule((PVOID*)addr, CallType) && addr > 0x0)
					{
						printf("[+] address of Reg Callback in kernel 0x%jx\n", addr);
					}
				}
				head = (DWORDLONG)pEntry->Flink;
				if (head == (DWORDLONG)CallbackListHeadOrCmpCallBackVector)
					Count = FALSE;
				if (!VMMDLL_MemRead(4, head, (PBYTE)pEntry, sizeof(_LIST_ENTRY)))
					break;
			} 
		}
		
	}
	return status;
}
NTSTATUS notify_list( PKKLL_M_MEMORY_GENERIC generics, SIZE_T cbGenerics, PUCHAR* ptr, PULONG pRoutineMax, CallBackType CallType)
{
	NTSTATUS status = STATUS_SUCCESS;
	PKKLL_M_NOTIFY_CALLBACK pNotifyCallback = new KKLL_M_NOTIFY_CALLBACK;
	ULONG i;

	if (!*ptr)
		status = notify_search(generics, cbGenerics, ptr, pRoutineMax, NULL);

	if (*ptr)
	{
		SystemModules Sys_modules;
		PUCHAR *Psp = new PUCHAR[*pRoutineMax];
		VMMDLL_MemRead(4, (DWORDLONG)*ptr, (PBYTE)Psp, *pRoutineMax*sizeof(PUCHAR));
		for (i = 0; NT_SUCCESS(status) && (i < *pRoutineMax); i++)
		{
			
			DWORDLONG PspAddr = KIWI_mask3bits(((PVOID*)Psp)[i]);
			if (VMMDLL_MemRead(4, PspAddr, (PBYTE)pNotifyCallback,sizeof(KKLL_M_NOTIFY_CALLBACK)))
			{
				if (!Sys_modules.PrintModule(pNotifyCallback->callback, CallType) && pNotifyCallback->callback > 0)
				{
					printf("[+] address of Process Callback in kernel 0x%jx\n", pNotifyCallback->callback);		
				}
				
			}
		}
	}
	return status;
}

NTSTATUS notify_search(PKKLL_M_MEMORY_GENERIC generics, SIZE_T cbGenerics, PUCHAR* ptr, PULONG pRoutineMax, PKKLL_M_MEMORY_OFFSETS* pOffsets)
{
	NTSTATUS status = STATUS_NOT_FOUND;
	PKKLL_M_MEMORY_GENERIC pGeneric;
	
	PUCHAR start, end;

	if (pGeneric = getGenericFromBuild(generics, cbGenerics))
	{
		
		start = (PUCHAR)Ken_info.Kr_Get_fun_addr(pGeneric->start);
		end = (PUCHAR)Ken_info.Kr_Get_fun_addr(pGeneric->end);

		if (start && end)
		{
			status = memory_genericPointerSearch(ptr, start, end, pGeneric->Search.Pattern, pGeneric->Search.Length, pGeneric->Offsets.off0);
			if (NT_SUCCESS(status))
			{
				if (pRoutineMax)
					*pRoutineMax = pGeneric->Offsets.off1;
				if (pOffsets)
					*pOffsets = &pGeneric->Offsets;
			}
		}
	}
	return status;
}
PKKLL_M_MEMORY_GENERIC getGenericFromBuild(PKKLL_M_MEMORY_GENERIC generics, SIZE_T cbGenerics)
{
	KiwiOsIndex = getWindowsIndex();
	SIZE_T i;
	for (i = 0; i < cbGenerics; i++)
		if (generics[i].OsIndex == KiwiOsIndex)
			return generics + i;
	return NULL;
}
KIWI_OS_INDEX getWindowsIndex()
{
	char value[255] = { 0x00 };
	DWORD BufferSize = 255;
	RegGetValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", "CurrentBuildNumber", RRF_RT_REG_SZ, NULL, &value, &BufferSize);
	//printf("[+] Windows Version %s Found\n", value);
	NtBuildNumber = atoi(value);

	if (NtBuildNumber > 19041) // forever 10 =)
		return KiwiOsIndex_10_2004;

	switch (NtBuildNumber)
	{
	case 2600:
		return KiwiOsIndex_XP;
		break;
	case 3790:
		return KiwiOsIndex_2K3;
		break;
	case 6000:
	case 6001:
	case 6002:
		return KiwiOsIndex_VISTA;
		break;
	case 7600:
	case 7601:
		return KiwiOsIndex_7;
		break;
	case 8102:
	case 8250:
	case 9200:
		return KiwiOsIndex_8;
	case 9431:
	case 9600:
		return KiwiOsIndex_BLUE;
		break;
	case 10240:
		return KiwiOsIndex_10_1507;
		break;
	case 10586:
		return KiwiOsIndex_10_1511;
		break;
	case 14393:
		return KiwiOsIndex_10_1607;
		break;
	case 15063:
		return KiwiOsIndex_10_1703;
		break;
	case 16299:
		return KiwiOsIndex_10_1709;
		break;
	case 17134:
		return KiwiOsIndex_10_1803;
		break;
	case 17763:
		return KiwiOsIndex_10_1809;
		break;
	case 18362:
		return KiwiOsIndex_10_1903;
		break;
	case 18363:
		return KiwiOsIndex_10_1909;
		break;
	case 19041:
		return KiwiOsIndex_10_2004;
		break;
	default:
		return KiwiOsIndex_UNK;
	}
}

NTSTATUS memory_search(const PUCHAR adresseBase, const PUCHAR adresseMaxMin, const UCHAR* pattern, PUCHAR* addressePattern, SIZE_T longueur)
{
	
	PBYTE ReadAddr = new BYTE[longueur+1];
	
	for (*addressePattern = adresseBase; (adresseMaxMin > adresseBase) ? (*addressePattern <= adresseMaxMin) : (*addressePattern >= adresseMaxMin); *addressePattern += (adresseMaxMin > adresseBase) ? 1 : -1)
	{
		memset(ReadAddr, 0, longueur);
		if (VMMDLL_MemRead(4, (DWORDLONG)*addressePattern, ReadAddr, (DWORD)longueur))
		{
			if (memcmp(ReadAddr, pattern, longueur) == 0) {
				//printf("Yeesss we find it\n");
				return STATUS_SUCCESS;
			}

		}
	}
	/*if not found start search randomly for 5K*/
	int i = 0;
	for (*addressePattern = adresseBase;i<5000 ; *addressePattern += 1)
	{
		memset(ReadAddr, 0, longueur);
		if (VMMDLL_MemRead(4, (DWORDLONG)*addressePattern, ReadAddr, (DWORD)longueur))
		{
			if (memcmp(ReadAddr, pattern, longueur) == 0) {
				//printf("second we find it\n");
				return STATUS_SUCCESS;
			}

		}
	}
	
	*addressePattern = NULL;
	return STATUS_NOT_FOUND;
}

NTSTATUS memory_genericPointerSearch(PUCHAR* addressePointeur, const PUCHAR adresseBase, const PUCHAR adresseMaxMin, const UCHAR* pattern, SIZE_T longueur, LONG offsetTo)
{
	NTSTATUS status = memory_search(adresseBase, adresseMaxMin, pattern, addressePointeur, longueur);
	if (NT_SUCCESS(status))
	{
		*addressePointeur += offsetTo;
#if defined(_M_X64)
		PBYTE data = new BYTE[sizeof(LONG)];
		VMMDLL_MemRead(4, (DWORDLONG)*addressePointeur, data, sizeof(LONG));
		*addressePointeur += sizeof(LONG) + *(PLONG)(data);
#elif defined(_M_IX86)
		*addressePointeur = *(PUCHAR*)(*addressePointeur);
#endif

		if (!*addressePointeur)
			status = STATUS_INVALID_HANDLE;
	}
	return status;
}