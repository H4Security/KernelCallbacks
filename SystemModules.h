#pragma once

#include <stdio.h>
#include <Windows.h>
#include <winternl.h>

#pragma comment(lib,"ntdll.lib")

class SystemModules
{
public:
    SystemModules();
	~SystemModules();
    BOOL PrintModule(PVOID*,CallBackType);
    BOOL HashFile(char*, LPCSTR);
private:
    #define BUFSIZE 1024
    #define SHA1LEN  20
    // These structures are copied from Process Hacker source code (ntldr.h)

    typedef struct _RTL_PROCESS_MODULE_INFORMATION
    {
        HANDLE Section;
        PVOID MappedBase;
        PVOID ImageBase;
        ULONG ImageSize;
        ULONG Flags;
        USHORT LoadOrderIndex;
        USHORT InitOrderIndex;
        USHORT LoadCount;
        USHORT OffsetToFileName;
        UCHAR FullPathName[256];
    } RTL_PROCESS_MODULE_INFORMATION, * PRTL_PROCESS_MODULE_INFORMATION;

    typedef struct _RTL_PROCESS_MODULES
    {
        ULONG NumberOfModules;
        RTL_PROCESS_MODULE_INFORMATION Modules[1];
    } RTL_PROCESS_MODULES, * PRTL_PROCESS_MODULES;
    PRTL_PROCESS_MODULES ModuleInfo;

};

BOOL SystemModules::PrintModule(PVOID* ProCallBack, CallBackType callType)
{
    ULONG i;
    for (i = 0; i < ModuleInfo->NumberOfModules; i++)
    {
        if (ProCallBack > ModuleInfo->Modules[i].ImageBase && (ProCallBack < (PVOID)((ULONG_PTR)ModuleInfo->Modules[i].ImageBase + (ULONG_PTR)ModuleInfo->Modules[i].ImageSize)))
        {
            char *digest= new char[(SHA1LEN * 2) + 1];
            
            if (!strncmp((const char*)ModuleInfo->Modules[i].FullPathName, "\\SystemRoot\\", 12))
            {
                char* path = new char[256];
                memcpy(path, "C:\\Windows", 10);
                
               size_t a = strlen((char*)&ModuleInfo->Modules[i].FullPathName[11]);
                strcpy_s(&path[10],a+1,(char*)&ModuleInfo->Modules[i].FullPathName[11]);
                strcpy_s((char*)&ModuleInfo->Modules[i].FullPathName, strlen(path) + 1, path);
                delete[] path;
                
            }
            if (!HashFile(digest, (LPCSTR)ModuleInfo->Modules[i].FullPathName))
            {
                DWORD value = int(digest[0]);
              
                size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                    NULL, value, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&digest, 0, NULL);


            }
            switch (callType)
            {
            case Process:
                printf("\nProCallBack: 0x%jx , Image full path: %s , Sha1: %s\n", ProCallBack, ModuleInfo->Modules[i].FullPathName, digest);
                break;
            case Image:
                printf("\nImage Creation CallBack: 0x%jx , Image full path: %s , Sha1: %s\n", ProCallBack, ModuleInfo->Modules[i].FullPathName, digest);
                break;
            case Thread:
                printf("\nThread Creation CallBack: 0x%jx , Image full path: %s , Sha1: %s\n", ProCallBack, ModuleInfo->Modules[i].FullPathName, digest);
                break;
            case Reg:
                printf("\nReg Creation CallBack: 0x%jx , Image full path: %s , Sha1: %s\n", ProCallBack, ModuleInfo->Modules[i].FullPathName, digest);
                break;
            default:
                break;
            }
           
            return TRUE;
        }
      
    }
    return FALSE;
};
SystemModules::SystemModules()
{
    NTSTATUS status;
   
    ModuleInfo = (PRTL_PROCESS_MODULES)VirtualAlloc(NULL, 1024 * 1024, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE); // Allocate memory for the module list

    if (!ModuleInfo)
    {
        printf("\nUnable to allocate memory for module list (%d)\n", GetLastError());
        
    }

    if (!NT_SUCCESS(status = NtQuerySystemInformation((SYSTEM_INFORMATION_CLASS)11, ModuleInfo, 1024 * 1024, NULL))) // 11 = SystemModuleInformation
    {
        printf("\nError: Unable to query module list (%#x)\n", status);

        VirtualFree(ModuleInfo, 0, MEM_RELEASE);
        
    }

    
}

BOOL SystemModules::HashFile(char* digest, LPCSTR filename)
{
    
    DWORD dwStatus = 0;
    BOOL bResult = FALSE;
    HCRYPTPROV hProv = 0;
    HCRYPTHASH hHash = 0;
    HANDLE hFile = NULL;
    BYTE rgbFile[BUFSIZE];
    DWORD cbRead = 0;
    BYTE rgbHash[SHA1LEN];
    DWORD cbHash = 0;
    CHAR rgbDigits[] = "0123456789abcdef";
    
    // Logic to check usage goes here.

    hFile = CreateFile(filename,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_SEQUENTIAL_SCAN,
        NULL);

    if (INVALID_HANDLE_VALUE == hFile)
    {
        dwStatus = GetLastError();
        digest[0] = dwStatus;
        return FALSE;
    }

    // Get handle to the crypto provider
    if (!CryptAcquireContext(&hProv,
        NULL,
        NULL,
        PROV_RSA_FULL,
        CRYPT_VERIFYCONTEXT))
    {
        dwStatus = GetLastError();
        digest[0] = dwStatus;
        return FALSE;
    }

    if (!CryptCreateHash(hProv, CALG_SHA1, 0, 0, &hHash))
    {
        dwStatus = GetLastError();
        digest[0] = dwStatus;
        return FALSE;
    }

    while (bResult = ReadFile(hFile, rgbFile, BUFSIZE,
        &cbRead, NULL))
    {
        if (0 == cbRead)
        {
            break;
        }

        if (!CryptHashData(hHash, rgbFile, cbRead, 0))
        {
            dwStatus = GetLastError();
            digest[0] = dwStatus;
            return FALSE;
        }
    }

    if (!bResult)
    {
        dwStatus = GetLastError();
        digest[0] = dwStatus;
        return FALSE;
    }

    cbHash = SHA1LEN;
    if (CryptGetHashParam(hHash, HP_HASHVAL, rgbHash, &cbHash, 0))
    {
       // printf("sha1 hash of file %s is: ", filename);
        for (DWORD i = 0, j = 0; i < cbHash * 2; i += 2, j++)
        {
            digest[i] = rgbDigits[rgbHash[j] >> 4];
            digest[i + 1] = rgbDigits[rgbHash[j] & 0xf];
        }
        digest[40] = '\0';
        
    }
    else
    {
        dwStatus = GetLastError();
        digest[0] = dwStatus;
        return FALSE;
    }

    CryptDestroyHash(hHash);
    CryptReleaseContext(hProv, 0);
    CloseHandle(hFile);

    return TRUE;

}
SystemModules::~SystemModules()
{
    VirtualFree(ModuleInfo, 0, MEM_RELEASE);
}