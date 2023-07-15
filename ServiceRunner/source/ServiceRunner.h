#include <string>
#include <Windows.h>

#pragma once

struct ServiceRunner
{
	static void svcStart(const char* svcname, bool *error, bool *svcIsRunning);
	static void svcStop(const char* svcname, bool *error, bool *svcIsRunning);
	static void svcWinApiStart(const std::wstring& svcname, DWORD *errorCode, bool *status = false);
	static void svcWinApiStop(const std::wstring& svcname, DWORD *errorCode, bool *status = false);
};