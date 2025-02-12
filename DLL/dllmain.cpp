// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include<Windows.h>
#include <string>
#include <sstream>
#include <vector>
#pragma execution_character_set("utf-8")//使用UTF-8
#pragma comment(lib, "gdi32.lib")
//定义
enum ZBID
{
	ZBID_DEFAULT = 0,
	ZBID_DESKTOP = 1,
	ZBID_UIACCESS = 2,
	ZBID_IMMERSIVE_IHM = 3,
	ZBID_IMMERSIVE_NOTIFICATION = 4,
	ZBID_IMMERSIVE_APPCHROME = 5,
	ZBID_IMMERSIVE_MOGO = 6,
	ZBID_IMMERSIVE_EDGY = 7,
	ZBID_IMMERSIVE_INACTIVEMOBODY = 8,
	ZBID_IMMERSIVE_INACTIVEDOCK = 9,
	ZBID_IMMERSIVE_ACTIVEMOBODY = 10,
	ZBID_IMMERSIVE_ACTIVEDOCK = 11,
	ZBID_IMMERSIVE_BACKGROUND = 12,
	ZBID_IMMERSIVE_SEARCH = 13,
	ZBID_GENUINE_WINDOWS = 14,
	ZBID_IMMERSIVE_RESTRICTED = 15,
	ZBID_SYSTEM_TOOLS = 16,
	ZBID_LOCK = 17,
	ZBID_ABOVELOCK_UX = 18,
};
typedef BOOL(WINAPI* SetWindowBand)(
	HWND hWnd,
	HWND hwndInsertAfter,
	DWORD dwBand
	);
typedef BOOL(WINAPI* GetWindowBand)(
	HWND hWnd,
	PDWORD pdwBand
	);

typedef BOOL(WINAPI* NtUserEnableIAMAccess)(
	ULONG64 key,
	BOOL enable
	);

// 全局变量
HINSTANCE g_hMod;
BYTE g_bDataJmp32[5] = { 0 };
BYTE g_bDataJmp64[12] = { 0 };
ULONG64 g_Key;
HWND g_Hwnd;
int g_Zorder;
int g_Band;
HANDLE g_hPipe;

DWORD WINAPI myMain(LPVOID lpParam);
BOOL setJMP();
BOOL resetJMP();
BOOL WINAPI NtUserEnableIAMAccessHook(ULONG64 key, BOOL enable);
VOID TryForceIAMAccessCallThread(LPVOID lpParam);

//主入口
BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		CreateThread(nullptr, 0, myMain, hModule, NULL, NULL);
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
DWORD WINAPI myMain(LPVOID lpParam)
{
	g_hPipe = CreateFile(
		L"\\\\.\\pipe\\GuoguaPinWindow",      // 管道名称
		GENERIC_READ | GENERIC_WRITE,         // 读写权限
		0,                                    // 不共享
		NULL,                                 // 默认安全属性
		OPEN_EXISTING,                        // 打开现有管道
		0,                                    // 默认属性
		NULL                                  // 不使用模板
	);

	if (g_hPipe == INVALID_HANDLE_VALUE)
	{
		return 0;
	}


	// 读取数据
	char buffer[256]{};
	DWORD bytesRead;
	if (!ReadFile(g_hPipe, buffer, sizeof(buffer), &bytesRead, NULL))
	{
		return 0;
	}
	std::vector<std::string> v;
	std::string s = buffer;
	std::wstring wMessage(s.begin(), s.end());
	std::istringstream iss(s);	// 输入流
	std::string token;			// 接收缓冲区
	while (std::getline(iss, token, '.'))	// 以split为分隔符
	{
		v.push_back(token);
	}
	if (v[0] != "PinWindow"&&v.size()!=4)
	{
		//连接错误
		const char* response = "EA";
		DWORD bytesWritten;
		WriteFile(g_hPipe, response, strlen(response), &bytesWritten, NULL);
		return 0;
	}
	//处理
	//将字符串转换为HWND
	uintptr_t ptr;
	std::stringstream ss;
	ss << std::hex << v[1];
	ss >> ptr;
	g_Hwnd = reinterpret_cast<HWND>(ptr);
	//将字符串转换为数字
	g_Zorder = std::atoi(v[2].c_str());
	g_Band = std::atoi(v[3].c_str());
	const char* response = "LA";
	DWORD bytesWritten;
	WriteFile(g_hPipe, response, strlen(response), &bytesWritten, NULL);
	setJMP();
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)&TryForceIAMAccessCallThread, NULL, NULL, NULL);
	return 0;
}

BOOL setJMP()
{
	NtUserEnableIAMAccess pNtUserEnableIAMAccess = NULL;
	DWORD dwOldProtect;

	pNtUserEnableIAMAccess = (NtUserEnableIAMAccess)
		GetProcAddress(GetModuleHandle(TEXT("user32.dll")), MAKEINTRESOURCEA(2510));

#ifndef _WIN64
	BYTE bDataJmp[5] = { 0xE9, 0x00, 0x00, 0x00, 0x00 };
	*(PINT_PTR)(bDataJmp + 1) = (INT_PTR)NtUserEnableIAMAccessHook -
		(INT_PTR)pNtUserEnableIAMAccess - 5;
	// 保存ZwQuerySystemInformation函数的前5个字节
	memcpy_s(g_bDataJmp32, sizeof(g_bDataJmp32), pNtUserEnableIAMAccess, sizeof(bDataJmp));
#else
	BYTE bDataJmp[12] = { 0x48, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xE0 };
	*(PINT_PTR)(bDataJmp + 2) = (INT_PTR)NtUserEnableIAMAccessHook;
	// 保存ZwQuerySystemInformation函数的前12个字节
	memcpy_s(g_bDataJmp64, sizeof(g_bDataJmp64), pNtUserEnableIAMAccess, sizeof(bDataJmp));
#endif

	// 修改页面保护属性，写入Jmp数据
	VirtualProtect(pNtUserEnableIAMAccess, sizeof(bDataJmp), PAGE_EXECUTE_READWRITE, &dwOldProtect);
	memcpy_s(pNtUserEnableIAMAccess, sizeof(bDataJmp), bDataJmp, sizeof(bDataJmp));
	VirtualProtect(pNtUserEnableIAMAccess, sizeof(bDataJmp), dwOldProtect, &dwOldProtect);
	const char* response = "LB";
	DWORD bytesWritten;
	WriteFile(g_hPipe, response, strlen(response), &bytesWritten, NULL);
	return TRUE;
}

BOOL resetJMP()
{
	NtUserEnableIAMAccess pNtUserEnableIAMAccess = NULL;
	DWORD dwOldProtect;

	pNtUserEnableIAMAccess = (NtUserEnableIAMAccess)
		GetProcAddress(GetModuleHandle(TEXT("user32.dll")), MAKEINTRESOURCEA(2510));

#ifndef _WIN64
	VirtualProtect(pNtUserEnableIAMAccess, sizeof(g_bDataJmp32), PAGE_EXECUTE_READWRITE, &dwOldProtect);
	memcpy_s(pNtUserEnableIAMAccess, sizeof(g_bDataJmp32), g_bDataJmp32, sizeof(g_bDataJmp32));
	VirtualProtect(pNtUserEnableIAMAccess, sizeof(g_bDataJmp32), dwOldProtect, &dwOldProtect);
#else
	VirtualProtect(pNtUserEnableIAMAccess, sizeof(g_bDataJmp64), PAGE_EXECUTE_READWRITE, &dwOldProtect);
	memcpy_s(pNtUserEnableIAMAccess, sizeof(g_bDataJmp64), g_bDataJmp64, sizeof(g_bDataJmp64));
	VirtualProtect(pNtUserEnableIAMAccess, sizeof(g_bDataJmp64), dwOldProtect, &dwOldProtect);
#endif
	const char* response = "LD";
	DWORD bytesWritten;
	WriteFile(g_hPipe, response, strlen(response), &bytesWritten, NULL);
	return TRUE;
}
//hook 函数
BOOL WINAPI NtUserEnableIAMAccessHook(ULONG64 key, BOOL enable)
{
	NtUserEnableIAMAccess pNtUserEnableIAMAccess = NULL;

	pNtUserEnableIAMAccess = (NtUserEnableIAMAccess)
		GetProcAddress(GetModuleHandle(TEXT("user32.dll")), MAKEINTRESOURCEA(2510));
	BOOL ret = FALSE;
	//恢复之后调用
	resetJMP();
	ret = pNtUserEnableIAMAccess(key, enable);
	if (ret)
	{
		g_Key = key;
		std::string s = "LE"+ std::to_string(g_Key);

		const char* response = s.c_str();
		DWORD bytesWritten;
		WriteFile(g_hPipe, response, strlen(response), &bytesWritten, NULL);//发送日志
		//设置线程为IAM线程
		if (pNtUserEnableIAMAccess(g_Key, TRUE))
		{
			//调用SetWindowBand
			SetWindowBand pSetWindowBand = (SetWindowBand)
				GetProcAddress(GetModuleHandle(TEXT("user32.dll")),"SetWindowBand");
			if (pSetWindowBand(g_Hwnd, (HWND)g_Zorder, (DWORD)g_Band))
			{
				const char* response = "finish";
				DWORD bytesWritten;
				WriteFile(g_hPipe, response, strlen(response), &bytesWritten, NULL);
			}
			else
			{
				std::string s = "EB" + std::to_string(GetLastError());

				const char* response = s.c_str();
				DWORD bytesWritten;
				WriteFile(g_hPipe, response, strlen(response), &bytesWritten, NULL);
			}
			//取消设置线程为IAM线程
			pNtUserEnableIAMAccess(g_Key, FALSE);
		}
		else
		{
			std::string s = "EC" + std::to_string(GetLastError());

			const char* response = s.c_str();
			DWORD bytesWritten;
			WriteFile(g_hPipe, response, strlen(response), &bytesWritten, NULL);
		}
		
	}
	else
	{

		std::string s = "ED" + std::to_string(key);
		const char* response = s.c_str();
		DWORD bytesWritten;
		WriteFile(g_hPipe, response, strlen(response), &bytesWritten, NULL);
		setJMP();
	}
	return ret;
}

VOID TryForceIAMAccessCallThread(LPVOID lpParam)
{
	
	//These 7 calls will force a call into EnableIAMAccess.
	auto hwndFore = GetForegroundWindow();
	auto hwndToFocus = FindWindow(L"Shell_TrayWnd", NULL);
	SetForegroundWindow(GetDesktopWindow()); //This in case Shell_TrayWnd is already focused
	Sleep(100);
	SetForegroundWindow(hwndToFocus); //Focus on the taskbar, should trigger EnableIAMAccess
	Sleep(100);
	SetForegroundWindow(hwndFore); //Restore focus.
	const char* response = "LC";
	DWORD bytesWritten;
	WriteFile(g_hPipe, response, strlen(response), &bytesWritten, NULL);
}
