#pragma once

#include "Helper.h"
#include "Kernel_Info.h"

Kernel_Info Ken_info;
int NtBuildNumber;
enum CallBackType { Process, Image, Thread, Reg };
PVOID* PspCreateThreadNotifyRoutine = NULL, * PspCreateProcessNotifyRoutine = NULL, * PspLoadImageNotifyRoutine = NULL;
ULONG PspCreateThreadNotifyRoutineMax = 0, PspCreateProcessNotifyRoutineMax = 0, PspLoadImageNotifyRoutineMax = 0;

PVOID* CallbackListHeadOrCmpCallBackVector = NULL;
PKKLL_M_MEMORY_OFFSETS pCmpCallBackOffsets = NULL;
POBJECT_DIRECTORY* ObpTypeDirectoryObject = NULL;
PKKLL_M_MEMORY_OFFSETS pObpTypeDirectoryObjectOffsets = NULL;

//PPSSETCREATEPROCESSNOTIFYROUTINEEX pPsSetCreateProcessNotifyRoutineEx = NULL;
POB_PRE_OPERATION_CALLBACK kkll_m_notify_fakePre = NULL;
POB_POST_OPERATION_CALLBACK kkll_m_notify_fakePost = NULL;

PKKLL_M_MEMORY_GENERIC getGenericFromBuild(PKKLL_M_MEMORY_GENERIC, SIZE_T);
NTSTATUS kkll_m_memory_search(const PUCHAR, const PUCHAR, const UCHAR*, PUCHAR*, SIZE_T);
NTSTATUS notify_list( PKKLL_M_MEMORY_GENERIC generics, SIZE_T cbGenerics, PUCHAR* ptr, PULONG pRoutineMax, CallBackType);
NTSTATUS notify_search(PKKLL_M_MEMORY_GENERIC generics, SIZE_T cbGenerics, PUCHAR* ptr, PULONG pRoutineMax, PKKLL_M_MEMORY_OFFSETS* pOffsets);
KIWI_OS_INDEX getWindowsIndex();
NTSTATUS notify_list_process (CallBackType);
NTSTATUS notify_list_image( CallBackType);
NTSTATUS notify_list_thread( CallBackType);
NTSTATUS notify_list_reg( CallBackType);
NTSTATUS memory_genericPointerSearch(PUCHAR*, const PUCHAR, const PUCHAR, const UCHAR*, SIZE_T, LONG);

#if defined(_M_X64)
UCHAR PTRN_W23_Thread[] = { 0x66, 0x90, 0x66, 0x90, 0x48, 0x8b, 0xce, 0xe8 };
UCHAR PTRN_WVI_Thread[] = { 0xbf, 0x40, 0x00, 0x00, 0x00, 0x48, 0x8b, 0xcb, 0xe8 };
UCHAR PTRN_W10_Thread[] = { 0x48, 0x8b, 0xcd, 0xe8 };
KKLL_M_MEMORY_GENERIC ThreadReferences[] = {
	{KiwiOsIndex_2K3,		{sizeof(PTRN_W23_Thread), PTRN_W23_Thread},				L"PsReferencePrimaryToken",				L"CcSetBcbOwnerPointer",			{ -4,  8}},
	{KiwiOsIndex_VISTA,		{sizeof(PTRN_WVI_Thread), PTRN_WVI_Thread},				L"PsDereferenceKernelStack",			L"ExRaiseAccessViolation",			{ -5, 64}},
	{KiwiOsIndex_7,			{sizeof(PTRN_WVI_Thread), PTRN_WVI_Thread},				L"RtlUnicodeToMultiByteSize",			L"MmLockPagableSectionByHandle",	{ -5, 64}},
	{KiwiOsIndex_8,			{sizeof(PTRN_WVI_Thread), PTRN_WVI_Thread},				L"PsAcquireProcessExitSynchronization",	L"FsRtlAddToTunnelCache",			{ -4, 64}},
	{KiwiOsIndex_BLUE,		{sizeof(PTRN_WVI_Thread), PTRN_WVI_Thread},				L"ObCreateObject",						L"NtFindAtom",						{ -5, 64}},
	{KiwiOsIndex_10_1507,	{sizeof(PTRN_W10_Thread), PTRN_W10_Thread},				L"PsRemoveCreateThreadNotifyRoutine",	L"PsRemoveLoadImageNotifyRoutine",	{ -8, 64}},
	{KiwiOsIndex_10_1511,	{sizeof(PTRN_W10_Thread), PTRN_W10_Thread},				L"PsRemoveCreateThreadNotifyRoutine",	L"PsRemoveLoadImageNotifyRoutine",	{ -8, 64}},
	{KiwiOsIndex_10_1607,	{sizeof(PTRN_W10_Thread), PTRN_W10_Thread},				L"PsRemoveCreateThreadNotifyRoutine",	L"PsRemoveLoadImageNotifyRoutine",	{ -8, 64}},
	{KiwiOsIndex_10_1703,	{sizeof(PTRN_W10_Thread), PTRN_W10_Thread},				L"PsRemoveCreateThreadNotifyRoutine",	L"PsRemoveLoadImageNotifyRoutine",	{ -8, 64}},
	{KiwiOsIndex_10_1709,	{sizeof(PTRN_W10_Thread), PTRN_W10_Thread},				L"PsRemoveCreateThreadNotifyRoutine",	L"PsRemoveLoadImageNotifyRoutine",	{ -8, 64}},
	{KiwiOsIndex_10_1803,	{sizeof(PTRN_W10_Thread), PTRN_W10_Thread},				L"PsRemoveCreateThreadNotifyRoutine",	L"PsRemoveLoadImageNotifyRoutine",	{ -8, 64}},
	{KiwiOsIndex_10_1809,	{sizeof(PTRN_W10_Thread), PTRN_W10_Thread},				L"PsRemoveCreateThreadNotifyRoutine",	L"PsRemoveLoadImageNotifyRoutine",	{ -8, 64}},
	{KiwiOsIndex_10_1903,	{sizeof(PTRN_W10_Thread), PTRN_W10_Thread},				L"PsRemoveCreateThreadNotifyRoutine",	L"PsRemoveLoadImageNotifyRoutine",	{ -8, 64}},
	{KiwiOsIndex_10_1909,	{sizeof(PTRN_W10_Thread), PTRN_W10_Thread},				L"PsRemoveCreateThreadNotifyRoutine",	L"PsRemoveLoadImageNotifyRoutine",	{ -8, 64}},
	{KiwiOsIndex_10_2004,	{sizeof(PTRN_W10_Thread), PTRN_W10_Thread},				L"PsRemoveCreateThreadNotifyRoutine",	L"PsRemoveLoadImageNotifyRoutine",	{ -8, 64}},
};
UCHAR PTRN_W23_Process[] = { 0x41, 0xbf, 0x08, 0x00, 0x00, 0x00, 0x49, 0x8b, 0xdf, 0x48, 0x8b, 0xce, 0xe8 };
UCHAR PTRN_WVI_Process[] = { 0x48, 0x89, 0x4c, 0x24, 0x40, 0x41, 0xbe, 0x40, 0x00, 0x00, 0x00, 0x48, 0x8d, 0x0c, 0xc1, 0xe8 };
UCHAR PTRN_WI7_Process[] = { 0x4c, 0x8b, 0xf9, 0x48, 0x8d, 0x0c, 0xc1, 0xe8 };
UCHAR PTRN_WI8_Process[] = { 0x8b, 0xc3, 0x48, 0x8d, 0x34, 0xc1, 0x48, 0x8b, 0xce, 0xe8 };
UCHAR PTRN_W81_Process[] = { 0x48, 0x8d, 0x04, 0xc1, 0x48, 0x89, 0x45, 0x70, 0x48, 0x8b, 0xc8, 0xe8 };
UCHAR PTRN_W10_1507_Process[] = { 0x8b, 0xc3, 0x45, 0x33, 0xc0, 0x48, 0x8b, 0xd6, 0x49, 0x8d, 0x0c, 0xc7, 0xe8 };
UCHAR PTRN_W10_1511_Process[] = { 0x49, 0x8d, 0x0c, 0xff, 0x45, 0x33, 0xc0, 0x48, 0x8b, 0xd6, 0xe8 };
UCHAR PTRN_W10_1607_Process[] = { 0x49, 0x8d, 0x0c, 0xfc, 0x45, 0x33, 0xc0, 0x48, 0x8b, 0xd6, 0xe8 };
UCHAR PTRN_W10_1703_Process[] = { 0x49, 0x8d, 0x0c, 0xdc, 0x45, 0x33, 0xc0, 0x48, 0x8b, 0xd6, 0xe8 };
UCHAR PTRN_W10_1709_Process[] = { 0x48, 0x8d, 0x0c, 0xdd, 0x00, 0x00, 0x00, 0x00, 0x45, 0x33, 0xc0, 0x49, 0x03, 0xcd, 0x48, 0x8b/*, 0xd6, 0xe8*/ };
KKLL_M_MEMORY_GENERIC ProcessReferences[] = {
	{KiwiOsIndex_2K3,		{sizeof(PTRN_W23_Process), PTRN_W23_Process},			L"PsReferencePrimaryToken",				L"CcSetBcbOwnerPointer",			{ -4,  8}},
	{KiwiOsIndex_VISTA,		{sizeof(PTRN_WVI_Process), PTRN_WVI_Process},			L"SeCreateAccessStateEx",				L"PsReferenceImpersonationToken",	{ -4, 64}},
	{KiwiOsIndex_7,			{sizeof(PTRN_WI7_Process), PTRN_WI7_Process},			L"RtlAreAllAccessesGranted",			L"RtlGetIntegerAtom",				{ -4, 64}},
	{KiwiOsIndex_8,			{sizeof(PTRN_WI8_Process), PTRN_WI8_Process},			L"PsAcquireProcessExitSynchronization",	L"FsRtlAddToTunnelCache",			{ -4, 64}},
	{KiwiOsIndex_BLUE,		{sizeof(PTRN_W81_Process), PTRN_W81_Process},			L"ObCreateObject",						L"NtFindAtom",						{ -4, 64}},
	{KiwiOsIndex_10_1507,	{sizeof(PTRN_W10_1507_Process), PTRN_W10_1507_Process},	L"PsSetCreateProcessNotifyRoutine",		L"KeRegisterProcessorChangeCallback",	{ -4, 64}},
	{KiwiOsIndex_10_1511,	{sizeof(PTRN_W10_1511_Process), PTRN_W10_1511_Process},	L"PsSetCreateProcessNotifyRoutine",		L"KeRegisterProcessorChangeCallback",	{ -4, 64}},
	{KiwiOsIndex_10_1607,	{sizeof(PTRN_W10_1607_Process), PTRN_W10_1607_Process},	L"PsSetCreateProcessNotifyRoutine",		L"KeRegisterProcessorChangeCallback",	{ -4, 64}},
	{KiwiOsIndex_10_1703,	{sizeof(PTRN_W10_1703_Process), PTRN_W10_1703_Process},	L"PsSetCreateProcessNotifyRoutine",		L"KeRegisterProcessorChangeCallback",	{ -4, 64}},
	{KiwiOsIndex_10_1709,	{sizeof(PTRN_W10_1709_Process), PTRN_W10_1709_Process},	L"PsSetCreateProcessNotifyRoutine",		L"RtlGetSystemBootStatus",				{ -4, 64}},
	{KiwiOsIndex_10_1803,	{sizeof(PTRN_W10_1709_Process), PTRN_W10_1709_Process},	L"PsSetCreateProcessNotifyRoutine",		L"EtwEnableTrace",					{ -4, 64}},
	{KiwiOsIndex_10_1809,	{sizeof(PTRN_W10_1709_Process), PTRN_W10_1709_Process},	L"PsSetCreateProcessNotifyRoutine",		L"IoCreateDriver",					{ -4, 64}},
	{KiwiOsIndex_10_1903,	{sizeof(PTRN_W10_1709_Process), PTRN_W10_1709_Process},	L"PsSetCreateProcessNotifyRoutine",		L"IoCreateDriver",					{ -4, 64}},
	{KiwiOsIndex_10_1909,	{sizeof(PTRN_W10_1709_Process), PTRN_W10_1709_Process},	L"PsSetCreateProcessNotifyRoutine",		L"IoCreateDriver",					{ -4, 64}},
	{KiwiOsIndex_10_2004,	{sizeof(PTRN_W10_1709_Process), PTRN_W10_1709_Process},	L"PsSetCreateProcessNotifyRoutine",		L"IoCreateDriver",					{ -4, 64}},
};
UCHAR PTRN_W23_Image[] = { 0x4c, 0x8b, 0xf1, 0x48, 0x89, 0x78, 0x20, 0x4d, 0x8b, 0xe0, 0x4c, 0x8b, 0xea, 0xbd, 0x08, 0x00, 0x00, 0x00 };
UCHAR PTRN_WVI_Image[] = { 0x4c, 0x8b, 0xf2, 0x41, 0x0f, 0xba, 0x6d, 0x00, 0x0a, 0x4c, 0x8b, 0xf9, 0x49, 0xc7, 0x00, 0x38, 0x00, 0x00, 0x00 };
UCHAR PTRN_WI7_Image[] = { 0x41, 0x0f, 0xba, 0x6d, 0x00, 0x0a, 0xbb, 0x01, 0x00, 0x00, 0x00, 0x4c, 0x8b, 0xf2, 0x4c, 0x8b, 0xf9 };
UCHAR PTRN_WI8_Image[] = { 0xbf, 0x08, 0x00, 0x00, 0x00, 0x41, 0x89, 0x06, 0x0f, 0x1f, 0x04, 0x00, 0x48, 0x8b, 0xcb, 0xe8 };
UCHAR PTRN_W81_Image[] = { 0x45, 0x33, 0xc0, 0x48, 0x8b, 0xd7, 0x48, 0x8d, 0x0c, 0xd9, 0xe8 };
UCHAR PTRN_W10_Image[] = { 0x45, 0x33, 0xc0, 0x48, 0x8d, 0x0c, 0xd9, 0x48, 0x8b, 0xd7, 0xe8 };
KKLL_M_MEMORY_GENERIC ImageReferences[] = {
	{KiwiOsIndex_2K3,		{sizeof(PTRN_W23_Image), PTRN_W23_Image},				L"PsRemoveLoadImageNotifyRoutine",		L"PsSetLegoNotifyRoutine",			{ -4,  8}},
	{KiwiOsIndex_VISTA,		{sizeof(PTRN_WVI_Image), PTRN_WVI_Image},				L"NtRequestPort",						L"RtlQueryTimeZoneInformation",		{ -4,  8}},
	{KiwiOsIndex_7,			{sizeof(PTRN_WI7_Image), PTRN_WI7_Image},				L"FsRtlReleaseFile",					L"IoSetPartitionInformationEx",		{ -4,  64}},
	{KiwiOsIndex_8,			{sizeof(PTRN_WI8_Image), PTRN_WI8_Image},				L"ExSizeOfRundownProtectionCacheAware",	L"MmProbeAndLockProcessPages",		{ -4,  8}},
	{KiwiOsIndex_BLUE,		{sizeof(PTRN_W81_Image), PTRN_W81_Image},				L"PsSetLoadImageNotifyRoutine",			L"PsSetCreateThreadNotifyRoutine",	{ -4,  64}},
	{KiwiOsIndex_10_1507,	{sizeof(PTRN_W10_Image), PTRN_W10_Image},				L"PsSetLoadImageNotifyRoutine",			L"PsSetCreateProcessNotifyRoutine",	{ -4,  64}},
	{KiwiOsIndex_10_1511,	{sizeof(PTRN_W10_Image), PTRN_W10_Image},				L"PsSetLoadImageNotifyRoutine",			L"PsSetCreateProcessNotifyRoutine",	{ -4,  64}},
	{KiwiOsIndex_10_1607,	{sizeof(PTRN_W10_Image), PTRN_W10_Image},				L"PsSetLoadImageNotifyRoutine",			L"SeRegisterLogonSessionTerminatedRoutineEx",	{ -4,  64}},
	{KiwiOsIndex_10_1703,	{sizeof(PTRN_W10_Image), PTRN_W10_Image},				L"PsSetLoadImageNotifyRoutine",			L"PsSetCreateProcessNotifyRoutine",	{ -4,  64}},
	{KiwiOsIndex_10_1709,	{sizeof(PTRN_W10_Image), PTRN_W10_Image},				L"PsSetLoadImageNotifyRoutine",			L"PsSetCreateProcessNotifyRoutine",	{ -4,  64}},
	{KiwiOsIndex_10_1803,	{sizeof(PTRN_W10_Image), PTRN_W10_Image},				L"PsSetLoadImageNotifyRoutine",			L"PsSetCreateProcessNotifyRoutine",	{ -4,  64}},
	{KiwiOsIndex_10_1809,	{sizeof(PTRN_W10_Image), PTRN_W10_Image},				L"PsSetLoadImageNotifyRoutineEx",		L"PsSetCreateProcessNotifyRoutine",	{ -4,  64}},
	{KiwiOsIndex_10_1903,	{sizeof(PTRN_W10_Image), PTRN_W10_Image},				L"PsSetLoadImageNotifyRoutineEx",		L"PsSetCreateProcessNotifyRoutine",	{ -4,  64}},
	{KiwiOsIndex_10_1909,	{sizeof(PTRN_W10_Image), PTRN_W10_Image},				L"PsSetLoadImageNotifyRoutineEx",		L"PsSetCreateProcessNotifyRoutine",	{ -4,  64}},
	{KiwiOsIndex_10_2004,	{sizeof(PTRN_W10_Image), PTRN_W10_Image},				L"PsSetLoadImageNotifyRoutineEx",		L"PsSetCreateProcessNotifyRoutine",	{ -4,  64}},
};
UCHAR PTRN_W23_Object[] = { 0x40, 0x32, 0xf6, 0x4c, 0x89, 0x7c, 0x24, 0x78, 0x45, 0x33, 0xff, 0x4d, 0x85, 0xe4 };
UCHAR PTRN_WVI_Object[] = { 0x41, 0x8a, 0xdf, 0x4c, 0x89, 0x7c, 0x24, 0x58, 0x4d, 0x3b, 0xe7, 0x88, 0x5c, 0x24, 0x66, 0x4c, 0x89, 0x7c, 0x24, 0x50, 0x49, 0x8b, 0xef, 0xc7, 0x44, 0x24, 0x68 };
UCHAR PTRN_WI7_Object[] = { 0x41, 0x8a, 0xde, 0x44, 0x88, 0x74, 0x24, 0x47, 0x88, 0x5c, 0x24, 0x46, 0x4c, 0x89, 0x74, 0x24, 0x38, 0x4c, 0x89, 0x74, 0x24, 0x30, 0x49, 0x8b, 0xee, 0xc7, 0x44, 0x24, 0x48 };
UCHAR PTRN_WI8_Object[] = { 0x41, 0x8a, 0xd8, 0x44, 0x88, 0x44, 0x24, 0x4f, 0x88, 0x5c, 0x24, 0x4e, 0x4c, 0x89, 0x44, 0x24, 0x38, 0x4d, 0x8b, 0xf0, 0x4c, 0x89, 0x44, 0x24, 0x30, 0xc7, 0x44, 0x24, 0x50 }; ////////// todo
UCHAR PTRN_W81_Object[] = { 0x41, 0x8a, 0xd8, 0x44, 0x88, 0x44, 0x24, 0x4f, 0x88, 0x5c, 0x24, 0x4e, 0x4c, 0x89, 0x44, 0x24, 0x38, 0x4d, 0x8b, 0xf0, 0x4c, 0x89, 0x44, 0x24, 0x30, 0xc7, 0x44, 0x24, 0x50 };
UCHAR PTRN_W10_Object[] = { 0x0f, 0xb7, 0x02, 0xff, 0xc9, 0x49, 0x03 };
KKLL_M_MEMORY_GENERIC ObjectReferences[] = {
	{KiwiOsIndex_2K3,		{sizeof(PTRN_W23_Object), PTRN_W23_Object},				L"ObCreateObjectType",					L"ObReferenceSecurityDescriptor",	{ -4, 0x078, 0x0d8}},
	{KiwiOsIndex_VISTA,		{sizeof(PTRN_WVI_Object), PTRN_WVI_Object},				L"ObRegisterCallbacks",					L"ObCreateObjectType",				{ -4, 0x010, 0x070, 0x228}},
	{KiwiOsIndex_7,			{sizeof(PTRN_WI7_Object), PTRN_WI7_Object},				L"ObUnRegisterCallbacks",				L"ObCreateObjectType",				{ -4, 0x010, 0x070, 0x0c0}},
	{KiwiOsIndex_8,			{sizeof(PTRN_WI8_Object), PTRN_WI8_Object},				L"ObCreateObjectType",					L"IoCreateController",				{ -4, 0x010, 0x070, 0x0c8}},
	{KiwiOsIndex_BLUE,		{sizeof(PTRN_W81_Object), PTRN_W81_Object},				L"ObCreateObjectType",					L"RtlRunOnceInitialize",			{ -4, 0x010, 0x070, 0x0c8}},
	{KiwiOsIndex_10_1507,	{sizeof(PTRN_W10_Object), PTRN_W10_Object},				L"ObCreateObjectType",					L"RtlRunOnceInitialize",			{ 25, 0x010, 0x070, 0x0c8}},
	{KiwiOsIndex_10_1511,	{sizeof(PTRN_W10_Object), PTRN_W10_Object},				L"ObCreateObjectType",					L"RtlRunOnceInitialize",			{ 25, 0x010, 0x070, 0x0c8}},
	{KiwiOsIndex_10_1607,	{sizeof(PTRN_W10_Object), PTRN_W10_Object},				L"ObCreateObjectType",					L"KseRegisterShim",					{ 25, 0x010, 0x070, 0x0c8}},
	{KiwiOsIndex_10_1703,	{sizeof(PTRN_W10_Object), PTRN_W10_Object},				L"ObCreateObjectType",					L"IoCreateDriver",					{ 25, 0x010, 0x070, 0x0c8}},
	{KiwiOsIndex_10_1709,	{sizeof(PTRN_W10_Object), PTRN_W10_Object},				L"ObCreateObjectType",					L"IoCreateDriver",					{ 25, 0x010, 0x070, 0x0c8}},
	{KiwiOsIndex_10_1803,	{sizeof(PTRN_W10_Object), PTRN_W10_Object},				L"ObCreateObjectType",					L"IoCreateDriver",					{ 25, 0x010, 0x070, 0x0c8}},
	{KiwiOsIndex_10_1809,	{sizeof(PTRN_W10_Object), PTRN_W10_Object},				L"ObCreateObjectType",					L"IoCreateDriver",					{ 25, 0x010, 0x070, 0x0c8}},
	{KiwiOsIndex_10_1903,	{sizeof(PTRN_W10_Object), PTRN_W10_Object},				L"ObCreateObjectType",					L"IoCreateDriver",					{ 25, 0x010, 0x070, 0x0c8}},
	{KiwiOsIndex_10_1909,	{sizeof(PTRN_W10_Object), PTRN_W10_Object},				L"ObCreateObjectType",					L"IoCreateDriver",					{ 25, 0x010, 0x070, 0x0c8}},
	{KiwiOsIndex_10_2004,	{sizeof(PTRN_W10_Object), PTRN_W10_Object},				L"ObCreateObjectType",					L"IoCreateDriver",					{ 25, 0x010, 0x070, 0x0c8}},
};
UCHAR PTRN_W23_Reg[] = { 0x49, 0x8d, 0x0c, 0xdc, 0x45, 0x33, 0xc0, 0x48, 0x8b, 0xd7, 0xe8 };
UCHAR PTRN_WVI_Reg[] = { 0x48, 0x8b, 0xf0, 0x48, 0x89, 0x44, 0x24, 0x38, 0x48, 0x85, 0xc0, 0x0f, 0x84 };
UCHAR PTRN_WI7_Reg[] = { 0x48, 0x8b, 0xf8, 0x48, 0x89, 0x44, 0x24, 0x28, 0x48, 0x3b, 0xc3, 0x0f, 0x84 };
UCHAR PTRN_WI8_Reg[] = { 0x49, 0x8b, 0x04, 0x24, 0x48, 0x3b, 0x43, 0x18, 0x74 };
UCHAR PTRN_W81_Reg[] = { 0x49, 0x8b, 0x04, 0x24, 0x48, 0x3b, 0x43, 0x18, 0x74 };
UCHAR PTRN_W10_Reg[] = { 0x48, 0x8b, 0xf8, 0x48, 0x89, 0x44, 0x24, 0x40, 0x48, 0x85, 0xc0, 0x0f, 0x84 };
KKLL_M_MEMORY_GENERIC RegReferences[] = {
	{KiwiOsIndex_2K3,		{sizeof(PTRN_W23_Reg), PTRN_W23_Reg},					L"CmRegisterCallback",					L"CmUnRegisterCallback",			{ -6}},
	{KiwiOsIndex_VISTA,		{sizeof(PTRN_WVI_Reg), PTRN_WVI_Reg},					L"CmUnRegisterCallback",				L"SeSetAuthorizationCallbacks",		{ -9, 0x030}},
	{KiwiOsIndex_7,			{sizeof(PTRN_WI7_Reg), PTRN_WI7_Reg},					L"CmUnRegisterCallback",				L"CmRegisterCallback",				{ -9, 0x028}},
	{KiwiOsIndex_8,			{sizeof(PTRN_WI8_Reg), PTRN_WI8_Reg},					L"CmSetCallbackObjectContext",			L"CmGetCallbackVersion",			{ -9, 0x028}},
	{KiwiOsIndex_BLUE,		{sizeof(PTRN_W81_Reg), PTRN_W81_Reg},					L"CmSetCallbackObjectContext",			L"DbgkLkmdUnregisterCallback",		{ -9, 0x028}},
	{KiwiOsIndex_10_1507,	{sizeof(PTRN_W10_Reg), PTRN_W10_Reg},					L"CmUnRegisterCallback",				L"FsRtlAllocateResource",			{ -9, 0x028}},
	{KiwiOsIndex_10_1511,	{sizeof(PTRN_W10_Reg), PTRN_W10_Reg},					L"CmUnRegisterCallback",				L"FsRtlAllocateResource",			{ -9, 0x028}},
	{KiwiOsIndex_10_1607,	{sizeof(PTRN_W10_Reg), PTRN_W10_Reg},					L"CmUnRegisterCallback",				L"FsRtlAllocateResource",			{ -9, 0x028}},
	{KiwiOsIndex_10_1703,	{sizeof(PTRN_W10_Reg), PTRN_W10_Reg},					L"CmUnRegisterCallback",				L"DbgkLkmdUnregisterCallback",		{ -9, 0x028}},
	{KiwiOsIndex_10_1709,	{sizeof(PTRN_W10_Reg), PTRN_W10_Reg},					L"CmUnRegisterCallback",				L"DbgkLkmdUnregisterCallback",		{ -9, 0x028}},
	{KiwiOsIndex_10_1803,	{sizeof(PTRN_W10_Reg), PTRN_W10_Reg},					L"CmUnRegisterCallback",				L"DbgkLkmdUnregisterCallback",		{ -9, 0x028}},
	{KiwiOsIndex_10_1809,	{sizeof(PTRN_W10_Reg), PTRN_W10_Reg},					L"CmUnRegisterCallback",				L"DbgkLkmdUnregisterCallback",		{ -9, 0x028}},
	{KiwiOsIndex_10_1903,	{sizeof(PTRN_W10_Reg), PTRN_W10_Reg},					L"CmUnRegisterCallback",				L"DbgkLkmdUnregisterCallback",		{ -9, 0x028}},
	{KiwiOsIndex_10_1909,	{sizeof(PTRN_W10_Reg), PTRN_W10_Reg},					L"CmUnRegisterCallback",				L"DbgkLkmdUnregisterCallback",		{ -9, 0x028}},
	{KiwiOsIndex_10_2004,	{sizeof(PTRN_W10_Reg), PTRN_W10_Reg},					L"CmUnRegisterCallback",				L"DbgkLkmdUnregisterCallback",		{ -9, 0x028}},
};
#elif defined(_M_IX86)
UCHAR PTRN_WXP_Thread[] = { 0xc7, 0x45, 0xa4, 0x08, 0x00, 0x00, 0x00, 0xff, 0x75, 0xbc, 0xe8 };
UCHAR PTRN_W23_Thread[] = { 0xc7, 0x45, 0xac, 0x08, 0x00, 0x00, 0x00, 0xff, 0x75, 0xd0, 0xe8 };
UCHAR PTRN_WVI_Thread[] = { 0xc7, 0x45, 0x0c, 0x40, 0x00, 0x00, 0x00, 0x53, 0xe8 };
UCHAR PTRN_WI7_Thread[] = { 0xc7, 0x45, 0x0c, 0x40, 0x00, 0x00, 0x00, 0x56, 0xe8 };
UCHAR PTRN_WI8_Thread[] = { 0xbb, 0x40, 0x00, 0x00, 0x00, 0x8d, 0x1b, 0xe8 };
UCHAR PTRN_W81_Thread[] = { 0xc7, 0x45, 0xa8, 0x40, 0x00, 0x00, 0x00, 0x8b, 0xcf, 0xe8 };
UCHAR PTRN_W10_Thread[] = { 0x33, 0xf6, 0x6a, 0x00, 0x8b, 0xd3, 0x8b, 0xcf, 0xe8 };
KKLL_M_MEMORY_GENERIC ThreadReferences[] = {	// PspCreateThreadNotifyRoutine
	{KiwiOsIndex_XP,		{sizeof(PTRN_WXP_Thread), PTRN_WXP_Thread},			L"NtSetInformationProcess",					L"LdrEnumResources",				{ -4,  8}},
	{KiwiOsIndex_2K3,		{sizeof(PTRN_W23_Thread), PTRN_W23_Thread},			L"NtSetInformationProcess",					L"LdrEnumResources",				{ -4,  8}},
	{KiwiOsIndex_VISTA,		{sizeof(PTRN_WVI_Thread), PTRN_WVI_Thread},			L"RtlValidSid",								L"NtOpenThreadTokenEx",				{ -4, 64}},
	{KiwiOsIndex_7,			{sizeof(PTRN_WI7_Thread), PTRN_WI7_Thread},			L"RtlCompareUnicodeStrings",				L"ObQueryNameString",				{ -4, 64}},
	{KiwiOsIndex_8,			{sizeof(PTRN_WI8_Thread), PTRN_WI8_Thread},			L"PsAssignImpersonationToken",				L"NtFindAtom",						{ -4, 64}},
	{KiwiOsIndex_BLUE,		{sizeof(PTRN_W81_Thread), PTRN_W81_Thread},			L"RtlGetIntegerAtom",						L"PsGetThreadSessionId",			{ -4, 64}},
	{KiwiOsIndex_10_1507,	{sizeof(PTRN_W10_Thread), PTRN_W10_Thread},			L"PsSetCreateProcessNotifyRoutine",			L"PoRegisterCoalescingCallback",	{ -4, 64}},
	{KiwiOsIndex_10_1511,	{sizeof(PTRN_W10_Thread), PTRN_W10_Thread},			L"PsSetCreateProcessNotifyRoutine",			L"PoRegisterCoalescingCallback",	{ -4, 64}},
	{KiwiOsIndex_10_1607,	{sizeof(PTRN_W10_Thread), PTRN_W10_Thread},			L"PsSetCreateProcessNotifyRoutine",			L"PoRegisterCoalescingCallback",	{ -4, 64}},
	{KiwiOsIndex_10_1703,	{sizeof(PTRN_W10_Thread), PTRN_W10_Thread},			L"PsSetCreateProcessNotifyRoutine",			L"PoRegisterCoalescingCallback",	{ -4, 64}},
	{KiwiOsIndex_10_1709,	{sizeof(PTRN_W10_Thread), PTRN_W10_Thread},			L"PsSetCreateProcessNotifyRoutine",			L"PoRegisterCoalescingCallback",	{ -4, 64}},
	{KiwiOsIndex_10_1809,	{sizeof(PTRN_W10_Thread), PTRN_W10_Thread},			L"PsSetCreateProcessNotifyRoutine",			L"IoRegisterDeviceInterface",		{ -4, 64}},
	{KiwiOsIndex_10_1903,	{sizeof(PTRN_W10_Thread), PTRN_W10_Thread},			L"PsSetCreateProcessNotifyRoutine",			L"IoRegisterDeviceInterface",		{ -4, 64}},
	{KiwiOsIndex_10_1909,	{sizeof(PTRN_W10_Thread), PTRN_W10_Thread},			L"PsSetCreateProcessNotifyRoutine",			L"IoRegisterDeviceInterface",		{ -4, 64}},
	{KiwiOsIndex_10_2004,	{sizeof(PTRN_W10_Thread), PTRN_W10_Thread},			L"PsSetCreateThreadNotifyRoutineEx",		L"IoCreateSymbolicLink",			{ -4, 64}},
};
UCHAR PTRN_WXP_Process[] = { 0xc7, 0x45, 0xb0, 0x08, 0x00, 0x00, 0x00, 0xff, 0x75, 0xcc, 0xe8 };
UCHAR PTRN_W23_Process[] = { 0xc7, 0x45, 0xb0, 0x08, 0x00, 0x00, 0x00, 0xff, 0x75, 0xc8, 0xe8 };
UCHAR PTRN_WVI_Process[] = { 0x89, 0x4d, 0x20, 0xff, 0x75, 0x18, 0xe8 };
UCHAR PTRN_WI7_Process[] = { 0x83, 0x65, 0x30, 0x00, 0xff, 0x75, 0x20, 0xe8 };
UCHAR PTRN_WI8_Process[] = { 0x83, 0xc0, 0x40, 0x89, 0x85, 0x58, 0xff, 0xff, 0xff, 0x8d, 0x85, 0x3c, 0xff, 0xff, 0xff, 0x89, 0x45, 0x9c, 0xbe };
UCHAR PTRN_W81_Process[] = { 0x89, 0x45, 0x9c, 0x83, 0x65, 0x8c, 0x00, 0x8b, 0xc8, 0xe8 };
UCHAR PTRN_W10_1507_Process[] = { 0x8b, 0xf7, 0x57, 0x8b, 0xd0, 0x8b, 0xcb, 0xe8 };
UCHAR PTRN_W10_1511_Process[] = { 0x33, 0xf6, 0x6a, 0x00, 0x8b, 0xd0, 0x8b, 0xcf, 0xe8 };
UCHAR PTRN_W10_1703_Process[] = { 0x33, 0xff, 0x6a, 0x00, 0x8b, 0xd0, 0x8b, 0xcb, 0xe8 };
KKLL_M_MEMORY_GENERIC ProcessReferences[] = {	// PspCreateProcessNotifyRoutine
	{KiwiOsIndex_XP,		{sizeof(PTRN_WXP_Process), PTRN_WXP_Process},		L"NtSetInformationProcess",					L"LdrEnumResources",				{ -4,  8}},
	{KiwiOsIndex_2K3,		{sizeof(PTRN_W23_Process), PTRN_W23_Process},		L"NtSetInformationProcess",					L"LdrEnumResources",				{ -4,  8}},
	{KiwiOsIndex_VISTA,		{sizeof(PTRN_WVI_Process), PTRN_WVI_Process},		L"RtlValidSid",								L"NtOpenThreadTokenEx",				{ -4, 64}},
	{KiwiOsIndex_7,			{sizeof(PTRN_WI7_Process), PTRN_WI7_Process},		L"RtlCompareUnicodeStrings",				L"ObQueryNameString",				{ -4, 64}},
	{KiwiOsIndex_8,			{sizeof(PTRN_WI8_Process), PTRN_WI8_Process},		L"PsAssignImpersonationToken",				L"NtFindAtom",						{ 19, 64}},
	{KiwiOsIndex_BLUE,		{sizeof(PTRN_W81_Process), PTRN_W81_Process},		L"RtlGetIntegerAtom",						L"PsGetThreadSessionId",			{ -4, 64}},
	{KiwiOsIndex_10_1507,	{sizeof(PTRN_W10_1507_Process), PTRN_W10_1507_Process},	L"PoRegisterCoalescingCallback",		L"RtlGenerateClass5Guid",			{ -4, 64}},
	{KiwiOsIndex_10_1511,	{sizeof(PTRN_W10_1511_Process), PTRN_W10_1511_Process},	L"PoRegisterCoalescingCallback",		L"RtlGenerateClass5Guid",			{ -4, 64}},
	{KiwiOsIndex_10_1607,	{sizeof(PTRN_W10_1511_Process), PTRN_W10_1511_Process},	L"PoRegisterCoalescingCallback",		L"PoRequestShutdownEvent",			{ -4, 64}},
	{KiwiOsIndex_10_1703,	{sizeof(PTRN_W10_1703_Process), PTRN_W10_1703_Process},	L"PoRegisterCoalescingCallback",		L"PoRequestShutdownEvent",			{ -4, 64}},
	{KiwiOsIndex_10_1709,	{sizeof(PTRN_W10_1703_Process), PTRN_W10_1703_Process},	L"PoRegisterCoalescingCallback",		L"PoRequestShutdownEvent",			{ -4, 64}},
	{KiwiOsIndex_10_1809,	{sizeof(PTRN_W10_1703_Process), PTRN_W10_1703_Process},	L"PoRegisterCoalescingCallback",		L"PoRequestShutdownEvent",			{ -4, 64}},
	{KiwiOsIndex_10_1903,	{sizeof(PTRN_W10_1703_Process), PTRN_W10_1703_Process},	L"PoRegisterCoalescingCallback",		L"PoRequestShutdownEvent",			{ -4, 64}},
	{KiwiOsIndex_10_1909,	{sizeof(PTRN_W10_1703_Process), PTRN_W10_1703_Process},	L"PoRegisterCoalescingCallback",		L"PoRequestShutdownEvent",			{ -4, 64}},
	{KiwiOsIndex_10_2004,	{sizeof(PTRN_W10_1703_Process), PTRN_W10_1703_Process},	L"PsSetCreateThreadNotifyRoutine",		L"IoCreateSymbolicLink",			{ -4, 64}},
};
UCHAR PTRN_WXP_Image[] = { 0x53, 0x56, 0x57, 0x6a, 0x08, 0xbf };
UCHAR PTRN_W23_Image[] = { 0x53, 0x56, 0x57, 0x6a, 0x08, 0xbf };
UCHAR PTRN_WVI_Image[] = { 0xc7, 0x45, 0xfc, 0x08, 0x00, 0x00, 0x00, 0xff, 0x75, 0x10, 0xe8 };
UCHAR PTRN_WI7_Image[] = { 0xc7, 0x45, 0xfc, 0x40, 0x00, 0x00, 0x00, 0xff, 0x75, 0x10, 0xe8 };
UCHAR PTRN_WI8_Image[] = { 0xbb, 0x08, 0x00, 0x00, 0x00, 0x8b, 0xff, 0xe8 };
UCHAR PTRN_W81_Image[] = { 0x33, 0xff, 0x6a, 0x00, 0x8b, 0xd6, 0x8b, 0xcb, 0xe8 };
UCHAR PTRN_W10_1507_Image[] = { 0x33, 0xf6, 0x6a, 0x00, 0x8b, 0xd3, 0x8b, 0xcf, 0xe8 };
UCHAR PTRN_W10_1511_Image[] = { 0x33, 0xf6, 0x53, 0x89, 0x45, 0xfc, 0x8b, 0xde, 0x56, 0x8b, 0xd7, 0x8b, 0xc8, 0xe8 };
UCHAR PTRN_W10_1709_Image[] = { 0x33, 0xf6, 0x57, 0x89, 0x45, 0x0c, 0x8b, 0xfe, 0x56, 0x8b, 0xd3, 0x8b, 0xc8, 0xe8 };
UCHAR PTRN_W10_2004_Image[] = { 0x33, 0xf6, 0x89, 0x45, 0x0c, 0x8b, 0xfe, 0x56, 0x8b, 0xd3, 0x8b, 0xc8, 0xe8 };
KKLL_M_MEMORY_GENERIC ImageReferences[] = {	// PspLoadImageNotifyRoutine
	{KiwiOsIndex_XP,		{sizeof(PTRN_WXP_Image), PTRN_WXP_Image},			L"PsRemoveLoadImageNotifyRoutine",			L"PsCreateSystemProcess",			{  6,  8}},
	{KiwiOsIndex_2K3,		{sizeof(PTRN_W23_Image), PTRN_W23_Image},			L"PsRemoveLoadImageNotifyRoutine",			L"PsCreateSystemThread",			{  6,  8}},
	{KiwiOsIndex_VISTA,		{sizeof(PTRN_WVI_Image), PTRN_WVI_Image},			L"RtlUpcaseUnicodeStringToCountedOemString",L"IoCheckShareAccessEx",			{ -4,  8}},
	{KiwiOsIndex_7,			{sizeof(PTRN_WI7_Image), PTRN_WI7_Image},			L"RtlCopySidAndAttributesArray",			L"SeImpersonateClientEx",			{ -4,  64}},
	{KiwiOsIndex_8,			{sizeof(PTRN_WI8_Image), PTRN_WI8_Image},			L"PsAssignImpersonationToken",				L"NtFindAtom",						{ -4,  8}},
	{KiwiOsIndex_BLUE,		{sizeof(PTRN_W81_Image), PTRN_W81_Image},			L"PsSetLoadImageNotifyRoutine",				L"ObRegisterCallbacks",				{ -4,  64}},
	{KiwiOsIndex_10_1507,	{sizeof(PTRN_W10_1507_Image), PTRN_W10_1507_Image},	L"PsSetLoadImageNotifyRoutine",				L"RtlGenerateClass5Guid",			{ -4,  64}},
	{KiwiOsIndex_10_1511,	{sizeof(PTRN_W10_1511_Image), PTRN_W10_1511_Image},	L"PsSetLoadImageNotifyRoutine",				L"RtlGenerateClass5Guid",			{ -4,  64}},
	{KiwiOsIndex_10_1607,	{sizeof(PTRN_W10_1511_Image), PTRN_W10_1511_Image},	L"PsSetLoadImageNotifyRoutine",				L"PsSetCreateProcessNotifyRoutine",	{ -4,  64}},
	{KiwiOsIndex_10_1703,	{sizeof(PTRN_W10_1511_Image), PTRN_W10_1511_Image},	L"PsSetLoadImageNotifyRoutine",				L"PsSetCreateProcessNotifyRoutine",	{ -4,  64}},
	{KiwiOsIndex_10_1709,	{sizeof(PTRN_W10_1709_Image), PTRN_W10_1709_Image},	L"PsSetLoadImageNotifyRoutine",				L"PsSetCreateProcessNotifyRoutine",	{ -4,  64}},
	{KiwiOsIndex_10_1809,	{sizeof(PTRN_W10_1709_Image), PTRN_W10_1709_Image},	L"PsSetLoadImageNotifyRoutineEx",			L"PsSetCreateProcessNotifyRoutine",	{ -4,  64}},
	{KiwiOsIndex_10_1903,	{sizeof(PTRN_W10_1709_Image), PTRN_W10_1709_Image},	L"PsSetLoadImageNotifyRoutineEx",			L"PsSetCreateProcessNotifyRoutine",	{ -4,  64}},
	{KiwiOsIndex_10_1909,	{sizeof(PTRN_W10_1709_Image), PTRN_W10_1709_Image},	L"PsSetLoadImageNotifyRoutineEx",			L"PsSetCreateProcessNotifyRoutine",	{ -4,  64}},
	{KiwiOsIndex_10_2004,	{sizeof(PTRN_W10_2004_Image), PTRN_W10_2004_Image},	L"PsSetLoadImageNotifyRoutineEx",			L"IoCreateSymbolicLink",			{ -4,  64}},
};
UCHAR PTRN_WXP_Object[] = { 0x3b, 0xfb, 0xc6, 0x45, 0xe6, 0x00, 0x89, 0x5d, 0xe0, 0x89, 0x5d, 0xdc, 0xc7, 0x45, 0xe8 };
UCHAR PTRN_W23_Object[] = { 0x3b, 0xfb, 0xc6, 0x45, 0xe6, 0x00, 0x89, 0x5d, 0xdc, 0x89, 0x5d, 0xd8, 0xc7, 0x45, 0xe8 };
UCHAR PTRN_WVI_Object[] = { 0x3b, 0xc3, 0x88, 0x5c, 0x24, 0x3a, 0x89, 0x5c, 0x24, 0x30, 0x89, 0x5c, 0x24, 0x2c, 0xc7, 0x44, 0x24, 0x3c };
UCHAR PTRN_WI7_Object[] = { 0xc6, 0x44, 0x24, 0x22, 0x00, 0xc6, 0x44, 0x24, 0x23, 0x00, 0x89, 0x74, 0x24, 0x18, 0x89, 0x74, 0x24, 0x14, 0xc7, 0x44, 0x24, 0x24 };
UCHAR PTRN_WI8_Object[] = { 0x33, 0xc0, 0x8b, 0xf8, 0x66, 0x89, 0x44, 0x24, 0x2a, 0x89, 0x44, 0x24, 0x1c, 0x89, 0x7c, 0x24, 0x18, 0xc7, 0x44, 0x24, 0x2c };
UCHAR PTRN_W81_Object[] = { 0x8d, 0x44, 0x24, 0x14, 0x50, 0x33, 0xc0, 0x89, 0x7c, 0x24, 0x18, 0x50, 0x6a, 0x40 };
UCHAR PTRN_W10_Object[] = { 0x66, 0x8b, 0x02, 0x49, 0x8d, 0x52, 0x02, 0x66, 0x83, 0xf8, 0x5c, 0x0f, 0x84 };
UCHAR PTRN_W10_1703_Object[] = { 0x0f, 0xb7, 0x07, 0x49, 0x03, 0xfa, 0x83, 0xf8, 0x5c, 0x0f, 0x84 };
UCHAR PTRN_W10_1809_Object[] = { 0x33, 0xc0, 0x89, 0x44, 0x24, 0x38, 0x89, 0x44, 0x24, 0x3c, 0x66, 0x89, 0x44, 0x24, 0x4a, 0xc7, 0x44, 0x24, 0x4c, 0x34, 0x12, 0xff, 0xff };
UCHAR PTRN_W10_2004_Object[] = { 0x8d, 0x44, 0x24, 0x4c, 0x50, 0x6a, 0x00, 0x6a, 0x00, 0x6a, 0x40, 0x8b, 0xd3, 0xe8 };
KKLL_M_MEMORY_GENERIC ObjectReferences[] = { // ObpTypeDirectoryObject
	{KiwiOsIndex_XP,		{sizeof(PTRN_WXP_Object), PTRN_WXP_Object},			L"ObCreateObjectType",						L"NtOpenThread",					{ -4, 0x040, 0x08c}},
	{KiwiOsIndex_2K3,		{sizeof(PTRN_W23_Object), PTRN_W23_Object},			L"ObCreateObjectType",						L"NtOpenThread",					{ -4, 0x040, 0x08c}},
	{KiwiOsIndex_VISTA,		{sizeof(PTRN_WVI_Object), PTRN_WVI_Object},			L"ObCreateObjectType",						L"RtlInvertRangeList",				{ -4, 0x008, 0x058, 0x138}},
	{KiwiOsIndex_7,			{sizeof(PTRN_WI7_Object), PTRN_WI7_Object},			L"ObCreateObjectType",						L"RtlInvertRangeList",				{ -4, 0x008, 0x058, 0x080}},
	{KiwiOsIndex_8,			{sizeof(PTRN_WI8_Object), PTRN_WI8_Object},			L"ObCreateObjectType",						L"SeTokenIsAdmin",					{ -4, 0x008, 0x058, 0x088}},
	{KiwiOsIndex_BLUE,		{sizeof(PTRN_W81_Object), PTRN_W81_Object},			L"ObCreateObjectType",						L"KseRegisterShim",					{ -4, 0x008, 0x058, 0x088}},
	{KiwiOsIndex_10_1507,	{sizeof(PTRN_W10_Object), PTRN_W10_Object},			L"ObCreateObjectType",						L"KseRegisterShim",					{ 23, 0x008, 0x058, 0x088}},
	{KiwiOsIndex_10_1511,	{sizeof(PTRN_W10_Object), PTRN_W10_Object},			L"ObCreateObjectType",						L"KseRegisterShim",					{ 23, 0x008, 0x058, 0x088}},
	{KiwiOsIndex_10_1607,	{sizeof(PTRN_W10_Object), PTRN_W10_Object},			L"ObCreateObjectType",						L"KseRegisterShim",					{ 23, 0x008, 0x058, 0x088}},
	{KiwiOsIndex_10_1703,	{sizeof(PTRN_W10_1703_Object), PTRN_W10_1703_Object},	L"ObCreateObjectType",					L"KseRegisterShim",					{ 21, 0x008, 0x058, 0x088}},
	{KiwiOsIndex_10_1709,	{sizeof(PTRN_W10_1703_Object), PTRN_W10_1703_Object},	L"ObCreateObjectType",					L"KseRegisterShim",					{ 21, 0x008, 0x058, 0x088}},
	{KiwiOsIndex_10_1809,	{sizeof(PTRN_W10_1809_Object), PTRN_W10_1809_Object},	L"ObCreateObjectType",					L"KseRegisterShim",					{ -4, 0x008, 0x058, 0x088}},
	{KiwiOsIndex_10_1903,	{sizeof(PTRN_W10_1809_Object), PTRN_W10_1809_Object},	L"ObCreateObjectType",					L"KseRegisterShim",					{ -4, 0x008, 0x058, 0x088}},
	{KiwiOsIndex_10_1909,	{sizeof(PTRN_W10_1809_Object), PTRN_W10_1809_Object},	L"ObCreateObjectType",					L"KseRegisterShim",					{ -4, 0x008, 0x058, 0x088}},
	{KiwiOsIndex_10_2004,	{sizeof(PTRN_W10_2004_Object), PTRN_W10_2004_Object},	L"ObCreateObjectType",					L"IoCreateDriver",					{ -4, 0x008, 0x058, 0x088}},
};
UCHAR PTRN_WXP_Reg[] = { 0x89, 0x7d, 0x10, 0x57, 0xff, 0x75, 0xfc, 0xff, 0x75, 0x08, 0xe8 };
UCHAR PTRN_W23_Reg[] = { 0x89, 0x5d, 0x08, 0x53, 0xff, 0x75, 0xfc, 0x57, 0xe8 };
UCHAR PTRN_WVI_Reg[] = { 0x8b, 0x03, 0x8b, 0x4b, 0x04, 0x3b, 0x46, 0x10, 0x75 };
UCHAR PTRN_WI7_Reg[] = { 0x8b, 0x03, 0x8b, 0x4b, 0x04, 0x3b, 0x46, 0x10, 0x75 };
UCHAR PTRN_WI8_Reg[] = { 0x53, 0x8d, 0x55, 0xd0, 0x8b, 0xce, 0xe8 };
UCHAR PTRN_W81_Reg[] = { 0x8b, 0x08, 0x8b, 0x40, 0x04, 0x3b, 0x4e, 0x10, 0x75 };
UCHAR PTRN_W10_Reg[] = { 0x8b, 0x4d, 0x0c, 0x8b, 0x01, 0x8b, 0x49, 0x04, 0x3b };
UCHAR PTRN_W10_1703_Reg[] = { 0x8b, 0x01, 0x8b, 0x49, 0x04, 0x3b, 0x46, 0x10 };//, 0x75};
KKLL_M_MEMORY_GENERIC RegReferences[] = { // CallbackListHead
	{KiwiOsIndex_XP,		{sizeof(PTRN_WXP_Reg), PTRN_WXP_Reg},			L"CmRegisterCallback",							L"FsRtlMdlReadDev",					{ -4}},
	{KiwiOsIndex_2K3,		{sizeof(PTRN_W23_Reg), PTRN_W23_Reg},			L"CmRegisterCallback",							L"FsRtlCopyRead",					{ -4}},
	{KiwiOsIndex_VISTA,		{sizeof(PTRN_WVI_Reg), PTRN_WVI_Reg},			L"CmSetCallbackObjectContext",					L"EmClientRuleRegisterNotification",{ -8, 0x01c}},
	{KiwiOsIndex_7,			{sizeof(PTRN_WI7_Reg), PTRN_WI7_Reg},			L"CmSetCallbackObjectContext",					L"DbgkLkmdUnregisterCallback",		{ -8, 0x01c}},
	{KiwiOsIndex_8,			{sizeof(PTRN_WI8_Reg), PTRN_WI8_Reg},			L"CmUnRegisterCallback",						L"FsRtlIsFatDbcsLegal",				{ -4, 0x01c}},
	{KiwiOsIndex_BLUE,		{sizeof(PTRN_W81_Reg), PTRN_W81_Reg},			L"CmSetCallbackObjectContext",					L"DbgkLkmdUnregisterCallback",		{ -8, 0x01c}},
	{KiwiOsIndex_10_1507,	{sizeof(PTRN_W10_Reg), PTRN_W10_Reg},			L"CmSetCallbackObjectContext",					L"CmUnRegisterCallback",			{ -8, 0x01c}},
	{KiwiOsIndex_10_1511,	{sizeof(PTRN_W10_Reg), PTRN_W10_Reg},			L"CmSetCallbackObjectContext",					L"CmUnRegisterCallback",			{ -8, 0x01c}},
	{KiwiOsIndex_10_1607,	{sizeof(PTRN_W10_Reg), PTRN_W10_Reg},			L"CmSetCallbackObjectContext",					L"CmUnRegisterCallback",			{ -8, 0x01c}},
	{KiwiOsIndex_10_1703,	{sizeof(PTRN_W10_1703_Reg), PTRN_W10_1703_Reg},	L"CmSetCallbackObjectContext",					L"CmUnRegisterCallback",			{ -8, 0x01c}},
	{KiwiOsIndex_10_1709,	{sizeof(PTRN_W10_1703_Reg), PTRN_W10_1703_Reg},	L"CmSetCallbackObjectContext",					L"CmUnRegisterCallback",			{ -8, 0x01c}},
	{KiwiOsIndex_10_1809,	{sizeof(PTRN_W10_1703_Reg), PTRN_W10_1703_Reg},	L"CmSetCallbackObjectContext",					L"CmUnRegisterCallback",			{ -8, 0x01c}},
	{KiwiOsIndex_10_1903,	{sizeof(PTRN_W10_1703_Reg), PTRN_W10_1703_Reg},	L"CmSetCallbackObjectContext",					L"CmUnRegisterCallback",			{ -8, 0x01c}},
	{KiwiOsIndex_10_1909,	{sizeof(PTRN_W10_1703_Reg), PTRN_W10_1703_Reg},	L"CmSetCallbackObjectContext",					L"CmUnRegisterCallback",			{ -8, 0x01c}},
	{KiwiOsIndex_10_2004,	{sizeof(PTRN_W10_1703_Reg), PTRN_W10_1703_Reg},	L"CmSetCallbackObjectContext",					L"CmUnRegisterCallback",			{-12, 0x01c}},
};

#endif 