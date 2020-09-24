#pragma once
#include <Windows.h>
#include <psapi.h>

extern "C" {



#define STATUS_SUCCESS                          ((NTSTATUS)0x00000000L) 
#define STATUS_NOT_FOUND                          ((NTSTATUS)0xC0000225L) 
#define STRSAFE_NO_TRUNCATION                          ((NTSTATUS)0x00001000L) 
#define OBJECT_HASH_TABLE_SIZE	37
#define CM_REG_MAX_CALLBACKS	100

    typedef struct _KERNEL_INFO {
        DWORDLONG OrgKernelStart;
        DWORDLONG OrgKernelEnd;
        HMODULE hKernel;
        MODULEINFO KernelInfo;
    };
    _KERNEL_INFO KERNEL_INFO;

    typedef ULONG OB_OPERATION;
    typedef struct _OBJECT_TYPE* POBJECT_TYPE;

    typedef enum _OB_PREOP_CALLBACK_STATUS {
        OB_PREOP_SUCCESS
    } OB_PREOP_CALLBACK_STATUS, * POB_PREOP_CALLBACK_STATUS;



    typedef struct _OB_PRE_CREATE_HANDLE_INFORMATION {
        _Inout_ ACCESS_MASK         DesiredAccess;
        _In_ ACCESS_MASK            OriginalDesiredAccess;
    } OB_PRE_CREATE_HANDLE_INFORMATION, * POB_PRE_CREATE_HANDLE_INFORMATION;

    typedef struct _OB_PRE_DUPLICATE_HANDLE_INFORMATION {
        _Inout_ ACCESS_MASK         DesiredAccess;
        _In_ ACCESS_MASK            OriginalDesiredAccess;
        _In_ PVOID                  SourceProcess;
        _In_ PVOID                  TargetProcess;
    } OB_PRE_DUPLICATE_HANDLE_INFORMATION, * POB_PRE_DUPLICATE_HANDLE_INFORMATION;

    typedef union _OB_PRE_OPERATION_PARAMETERS {
        _Inout_ OB_PRE_CREATE_HANDLE_INFORMATION        CreateHandleInformation;
        _Inout_ OB_PRE_DUPLICATE_HANDLE_INFORMATION     DuplicateHandleInformation;
    } OB_PRE_OPERATION_PARAMETERS, * POB_PRE_OPERATION_PARAMETERS;



    typedef struct _OB_POST_CREATE_HANDLE_INFORMATION {
        _In_ ACCESS_MASK            GrantedAccess;
    } OB_POST_CREATE_HANDLE_INFORMATION, * POB_POST_CREATE_HANDLE_INFORMATION;

    typedef struct _OB_POST_DUPLICATE_HANDLE_INFORMATION {
        _In_ ACCESS_MASK            GrantedAccess;
    } OB_POST_DUPLICATE_HANDLE_INFORMATION, * POB_POST_DUPLICATE_HANDLE_INFORMATION;

    typedef union _OB_POST_OPERATION_PARAMETERS {
        _In_ OB_POST_CREATE_HANDLE_INFORMATION       CreateHandleInformation;
        _In_ OB_POST_DUPLICATE_HANDLE_INFORMATION    DuplicateHandleInformation;
    } OB_POST_OPERATION_PARAMETERS, * POB_POST_OPERATION_PARAMETERS;

    typedef struct _OB_PRE_OPERATION_INFORMATION {
        _In_ OB_OPERATION           Operation;
        union {
            _In_ ULONG Flags;
            struct {
                _In_ ULONG KernelHandle : 1;
                _In_ ULONG Reserved : 31;
            };
        };
        _In_ PVOID                         Object;
        _In_ POBJECT_TYPE                  ObjectType;
        _Out_ PVOID                        CallContext;
        _In_ POB_PRE_OPERATION_PARAMETERS  Parameters;
    } OB_PRE_OPERATION_INFORMATION, * POB_PRE_OPERATION_INFORMATION;

    typedef OB_PREOP_CALLBACK_STATUS
    (*POB_PRE_OPERATION_CALLBACK) (
        _In_ PVOID RegistrationContext,
        _Inout_ POB_PRE_OPERATION_INFORMATION OperationInformation
        );

    typedef struct _OB_POST_OPERATION_INFORMATION {
        _In_ OB_OPERATION  Operation;
        union {
            _In_ ULONG Flags;
            struct {
                _In_ ULONG KernelHandle : 1;
                _In_ ULONG Reserved : 31;
            };
        };
        _In_ PVOID                          Object;
        _In_ POBJECT_TYPE                   ObjectType;
        _In_ PVOID                          CallContext;
        _In_ NTSTATUS                       ReturnStatus;
        _In_ POB_POST_OPERATION_PARAMETERS  Parameters;
    } OB_POST_OPERATION_INFORMATION, * POB_POST_OPERATION_INFORMATION;
    typedef VOID
    (*POB_POST_OPERATION_CALLBACK) (
        _In_ PVOID RegistrationContext,
        _In_ POB_POST_OPERATION_INFORMATION OperationInformation
        );
    typedef struct _OBJECT_DIRECTORY_ENTRY {
        struct	_OBJECT_DIRECTORY_ENTRY* ChainLink;
        PVOID								Object;
    } OBJECT_DIRECTORY_ENTRY, * POBJECT_DIRECTORY_ENTRY;
    typedef struct _OBJECT_DIRECTORY {
        POBJECT_DIRECTORY_ENTRY		HashBuckets[OBJECT_HASH_TABLE_SIZE];
        /* ... */
    } OBJECT_DIRECTORY, * POBJECT_DIRECTORY;
    typedef struct _KKLL_M_NOTIFY_CALLBACK {
#if defined(_M_IX86)
        ULONG unk0;
#endif
        PVOID* callback;
    } KKLL_M_NOTIFY_CALLBACK, * PKKLL_M_NOTIFY_CALLBACK;


#if defined(_M_X64) || defined(_M_ARM64) // TODO:ARM64
#define EX_FAST_REF_MASK	0x0f
#elif defined(_M_IX86)
#define EX_FAST_REF_MASK	0x07
#endif

#define KIWI_mask3bits(addr)	 (((ULONG_PTR) (addr)) & ~7)


    typedef struct _KKLL_M_MEMORY_PATTERN {
        DWORD Length;
        PUCHAR Pattern;
    } KKLL_M_MEMORY_PATTERN, * PKKLL_M_MEMORY_PATTERN;

    typedef struct _KKLL_M_MEMORY_OFFSETS {
        LONG off0;
        LONG off1;
        LONG off2;
        LONG off3;
        LONG off4;
        LONG off5;
        LONG off6;
        LONG off7;
        LONG off8;
        LONG off9;
    } KKLL_M_MEMORY_OFFSETS, * PKKLL_M_MEMORY_OFFSETS;
    typedef enum _KIWI_OS_INDEX {
        KiwiOsIndex_UNK = 0,
        KiwiOsIndex_XP = 1,
        KiwiOsIndex_2K3 = 2,
        KiwiOsIndex_VISTA = 3,
        KiwiOsIndex_7 = 4,
        KiwiOsIndex_8 = 5,
        KiwiOsIndex_BLUE = 6,
        KiwiOsIndex_10_1507 = 7,
        KiwiOsIndex_10_1511 = 8,
        KiwiOsIndex_10_1607 = 9,
        KiwiOsIndex_10_1703 = 10,
        KiwiOsIndex_10_1709 = 11,
        KiwiOsIndex_10_1803 = 12,
        KiwiOsIndex_10_1809 = 13,
        KiwiOsIndex_10_1903 = 14,
        KiwiOsIndex_10_1909 = 15,
        KiwiOsIndex_10_2004 = 16,
        KiwiOsIndex_MAX = 17,
    } KIWI_OS_INDEX, * PKIWI_OS_INDEX;

    KIWI_OS_INDEX KiwiOsIndex;

    typedef struct _KKLL_M_MEMORY_GENERIC {
        KIWI_OS_INDEX OsIndex;
        KKLL_M_MEMORY_PATTERN Search;
        PWCHAR start;
        PWCHAR end;
        KKLL_M_MEMORY_OFFSETS Offsets;
    } KKLL_M_MEMORY_GENERIC, * PKKLL_M_MEMORY_GENERIC;

    typedef struct _KIWI_BUFFER {
        size_t* szBuffer;
        PWSTR* Buffer;
    } KIWI_BUFFER, * PKIWI_BUFFER;
}