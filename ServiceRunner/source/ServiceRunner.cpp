#include <Windows.h>
#include <string>
#include "ServiceRunner.h"

// Run service use Command Prompt
void ServiceRunner::svcStart(const char* svcname, bool *error, bool *svcIsRunning)
{
	std::string command = std::string("NET START ") + svcname;

	if (system(command.c_str()) == 1) {
		*error = false;
		*svcIsRunning = false;
	} else {
		*error = true;
		*svcIsRunning = true;
	}
}

// Stop service use Command Prompt
void ServiceRunner::svcStop(const char* svcname, bool *error, bool *svcIsRunning)
{
	std::string command = std::string("NET STOP ") + svcname;

	if (system(command.c_str()) == 1) {
		*error = false;
		*svcIsRunning = true;
	} else {
		*error = true;
		*svcIsRunning = false;
	}
}

// Run service use WinApi
void ServiceRunner::svcWinApiStart(const std::wstring& svcname, DWORD *errorCode, bool *status)
{
	std::string svcnamestr(svcname.begin(), svcname.end());
    LPCSTR svcnameCStr = svcnamestr.c_str();

	SC_HANDLE ServiceControlManager = OpenSCManager(nullptr, nullptr, SC_MANAGER_CONNECT);
    if (ServiceControlManager)
    {
		SC_HANDLE service = OpenService(ServiceControlManager, svcnameCStr, SERVICE_START);
        if (service)
        {
            if (StartService(service, 0, nullptr))
            {
                *status = true;
            }
            else
            {
                *errorCode = GetLastError();
            }
        }
        else
        {
            *errorCode = GetLastError();
        }

		CloseServiceHandle(service);
    }
    else
    {
		*errorCode = GetLastError();
    }

	CloseServiceHandle(ServiceControlManager);
}

// Stop service use WinApi
void ServiceRunner::svcWinApiStop(const std::wstring& svcname, DWORD *errorCode, bool *status)
{
	std::string svcnamestr(svcname.begin(), svcname.end());
    LPCSTR svcnameCStr = svcnamestr.c_str();

	SC_HANDLE ServiceControlManager = OpenSCManager(nullptr, nullptr, SC_MANAGER_CONNECT);
    if (ServiceControlManager)
    {
		SC_HANDLE service = OpenService(ServiceControlManager, svcnameCStr, SERVICE_STOP | SERVICE_QUERY_STATUS);
        if (service)
        {
			SERVICE_STATUS svcStatus;
			if (ControlService(service, SERVICE_CONTROL_STOP, &svcStatus))
            {
                *status = false;
            }
            else
            {
                *errorCode = GetLastError();
            }
        }
        else
        {
            *errorCode = GetLastError();
        }

		CloseServiceHandle(service);
    }
    else
    {
		*errorCode = GetLastError();
    }

	CloseServiceHandle(ServiceControlManager);
}

void ServiceRunner::svcCheckStatus(const std::wstring& svcname, DWORD *errorCode, bool *status)
{
	std::string svcnamestr(svcname.begin(), svcname.end());
    LPCSTR svcnameCStr = svcnamestr.c_str();

	SC_HANDLE ServiceControlManager = OpenSCManager(nullptr, nullptr, SC_MANAGER_CONNECT);
	if (ServiceControlManager) {
		SC_HANDLE service = OpenService(ServiceControlManager, svcnameCStr, SERVICE_QUERY_STATUS);
		if (service) {
			SERVICE_STATUS svcStatus;

			if (QueryServiceStatus(service, &svcStatus) == 0) {
				CloseServiceHandle(service);
				CloseServiceHandle(ServiceControlManager);

				return;
			}

			if (svcStatus.dwCurrentState == SERVICE_RUNNING) {
				*status = true;
			} else {
				*status = false;
			} 
		} else
        {
            *errorCode = GetLastError();
        }

		CloseServiceHandle(service);
	} else
    {
        *errorCode = GetLastError();
    }

	CloseServiceHandle(ServiceControlManager);
}

bool ServiceRunner::svcCheckRegistered(const std::wstring& svcname)
{
	std::string svcnamestr(svcname.begin(), svcname.end());
    LPCSTR svcnameCStr = svcnamestr.c_str();
	bool isRegistered = false;

	SC_HANDLE ServiceControlManager = OpenSCManager(nullptr, nullptr, SC_MANAGER_CONNECT);
	if (ServiceControlManager) {
		SC_HANDLE service = OpenService(ServiceControlManager, svcnameCStr, SERVICE_QUERY_STATUS);
		if (service) {
			CloseServiceHandle(service);
			CloseServiceHandle(ServiceControlManager);

			isRegistered = true;
		} else {
			CloseServiceHandle(service);
			CloseServiceHandle(ServiceControlManager);

			isRegistered = false;
		}
	}

	return isRegistered;
}