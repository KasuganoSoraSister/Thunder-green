#include "stdafx.h"
#include "string"
#include "Hook.h"

using namespace std;

BOOL(
WINAPI*
addr)(
	_In_opt_ LPCWSTR lpApplicationName,
	_Inout_opt_ LPWSTR lpCommandLine,
	_In_opt_ LPSECURITY_ATTRIBUTES lpProcessAttributes,
	_In_opt_ LPSECURITY_ATTRIBUTES lpThreadAttributes,
	_In_ BOOL bInheritHandles,
	_In_ DWORD dwCreationFlags,
	_In_opt_ LPVOID lpEnvironment,
	_In_opt_ LPCWSTR lpCurrentDirectory,
	_In_ LPSTARTUPINFOW lpStartupInfo,
	_Out_ LPPROCESS_INFORMATION lpProcessInformation
);

BOOL
WINAPI
MyCreateProcessW(
	_In_opt_ LPCWSTR lpApplicationName,
	_Inout_opt_ LPWSTR lpCommandLine,
	_In_opt_ LPSECURITY_ATTRIBUTES lpProcessAttributes,
	_In_opt_ LPSECURITY_ATTRIBUTES lpThreadAttributes,
	_In_ BOOL bInheritHandles,
	_In_ DWORD dwCreationFlags,
	_In_opt_ LPVOID lpEnvironment,
	_In_opt_ LPCWSTR lpCurrentDirectory,
	_In_ LPSTARTUPINFOW lpStartupInfo,
	_Out_ LPPROCESS_INFORMATION lpProcessInformation
) {
	
	if (wcsstr(lpCommandLine, L"SDK") <= 0) {
		return false;
	}

	BYTE b_old[8] = { 0x8B,0xFF,0x55,0x8B,0xEC,0x6A,0x00 };
	BYTE b_new[8];
	ReadProcessMemory((HANDLE)-1, addr, b_new, 7, 0);

	WriteProcessMemory((HANDLE)-1, addr, b_old, 7, 0);

	bool result = addr(lpApplicationName,
		lpCommandLine,
		lpProcessAttributes,
		lpThreadAttributes,
		bInheritHandles,
		dwCreationFlags,
		lpEnvironment,
		lpCurrentDirectory,
		lpStartupInfo,
		lpProcessInformation
	);
	WriteProcessMemory((HANDLE)-1, addr, b_new, 7, 0);

	return result;
}

int p_window;
int retn_addr;

void __declspec(naked) HookGetWindowPoint() {
	_asm mov p_window, ecx;
	_asm pushad;
	_asm pushfd;
	if (p_window && *(int*)(p_window + 0xD4) == 1235) {
		*(int *)(p_window + 0xD4) = 360;
		SetWindowPos(FindWindow(L"XLUEFrameHostWnd", L"—∏¿◊"), 0, 360, 690, 360, 690, SWP_NOMOVE);
	}
	_asm popfd;
	_asm popad;
	_asm push ebp;
	_asm mov ebp, esp;
	_asm sub esp, 0x88;
	_asm push retn_addr;
	_asm retn;
}

DWORD WINAPI MonitorWindow(LPVOID) {
	HWND hWnd = NULL;
	RECT r;
	bool IsLoad = false;
	for (;;) {
		if (hWnd = FindWindow(L"XLUEFrameHostWnd", L"—∏¿◊")) {
			//MessageBox(0, L"’“µΩ¥∞ø⁄",0,0);
			//SetWindowPos(hWnd, 0, 360, 690, 360, 690, SWP_NOMOVE);
			//MoveWindow(hWnd,0,0,360,690,true);
			if (!IsLoad) IsLoad = true;

			GetWindowRect(hWnd, &r);
			if (r.right - r.left > 360) {
				SetWindowPos(hWnd, 0, 360, 690, 360, 690, SWP_NOMOVE);
			} 
		}
		if (IsLoad == false) {
			Sleep(100);
		}
		else
			Sleep(3000);
	}
}


DWORD WINAPI StartHook(LPVOID) {
	BYTE hook[6];
	hook[0] = 0x68;
	hook[5] = 0xC3;

	CreateThread(0, 0, MonitorWindow, 0, 0, 0);

	addr = (BOOL(
		WINAPI*)(
			_In_opt_ LPCWSTR lpApplicationName,
			_Inout_opt_ LPWSTR lpCommandLine,
			_In_opt_ LPSECURITY_ATTRIBUTES lpProcessAttributes,
			_In_opt_ LPSECURITY_ATTRIBUTES lpThreadAttributes,
			_In_ BOOL bInheritHandles,
			_In_ DWORD dwCreationFlags,
			_In_opt_ LPVOID lpEnvironment,
			_In_opt_ LPCWSTR lpCurrentDirectory,
			_In_ LPSTARTUPINFOW lpStartupInfo,
			_Out_ LPPROCESS_INFORMATION lpProcessInformation
			))GetProcAddress(GetModuleHandle(L"KernelBase.dll"), "CreateProcessW");

	if (addr == NULL )
		addr = (BOOL(
			WINAPI*)(
				_In_opt_ LPCWSTR lpApplicationName,
				_Inout_opt_ LPWSTR lpCommandLine,
				_In_opt_ LPSECURITY_ATTRIBUTES lpProcessAttributes,
				_In_opt_ LPSECURITY_ATTRIBUTES lpThreadAttributes,
				_In_ BOOL bInheritHandles,
				_In_ DWORD dwCreationFlags,
				_In_opt_ LPVOID lpEnvironment,
				_In_opt_ LPCWSTR lpCurrentDirectory,
				_In_ LPSTARTUPINFOW lpStartupInfo,
				_Out_ LPPROCESS_INFORMATION lpProcessInformation
				))GetProcAddress(GetModuleHandle(L"Kernel32.dll"), "CreateProcessW");

	if (addr == NULL)
		MessageBox(0, L"º”‘ÿ ß∞‹", 0, 0);

	*(int*)(hook + 1) = (int)MyCreateProcessW;
	WriteProcessMemory((HANDLE)-1,
		addr,
		hook,
		6,
		0
	);

	HMODULE hModule = GetModuleHandle(L"XLUE.dll");//034D6060    55              push ebp
	DWORD oldProtect;
	if (hModule) {
		BYTE buffer;
		WCHAR temp[100];

		ReadProcessMemory((HANDLE)-1, (BYTE*)hModule + 0x176060, &buffer, 1, 0);


		if (buffer == 0x55) {
			*(int*)(hook + 1) = (int)HookGetWindowPoint;
			retn_addr = (int)hModule + 0x176069;
			WriteProcessMemory((HANDLE)-1,
				(BYTE*)hModule + 0x176060,
				hook,
				6,
				0
			);
		}
	}

	return 0;
}