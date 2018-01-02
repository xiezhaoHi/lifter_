#pragma once

#include "Windows.h"  

#define SERVICE_NAME L"Lifter_server_mscv"

SERVICE_STATUS ServiceStatus;
SERVICE_STATUS_HANDLE hServiceStatusHandle;
void WINAPI service_main(int argc, char** argv);
void WINAPI ServiceHandler(DWORD fdwControl);

TCHAR szSvcName[80];
SC_HANDLE schSCManager;
SC_HANDLE schService;
int uaquit;

HANDLE g_quitEvent = NULL;
DWORD WINAPI srv_core_thread(LPVOID para)
{
	int i = 0;
	g_quitEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (NULL == g_quitEvent)
		return -1;
	while (1)
	{
		server_manager::GetInstance()->start_server();
		WaitForSingleObject(g_quitEvent,INFINITE);
		CloseHandle(g_quitEvent);
		server_manager::GetInstance()->stop_server();
	}
	return 1;
}


void WINAPI ServiceHandler(DWORD fdwControl)
{
	switch (fdwControl)
	{
	case SERVICE_CONTROL_STOP:
	case SERVICE_CONTROL_SHUTDOWN:
		ServiceStatus.dwWin32ExitCode = 0;
		ServiceStatus.dwCurrentState = SERVICE_STOPPED;
		ServiceStatus.dwCheckPoint = 0;
		ServiceStatus.dwWaitHint = 0;
		uaquit = 1;
		//add you quit code here  
		//停止服务
		SetEvent(g_quitEvent);

		break;
	default:
		return;
	};
	if (!SetServiceStatus(hServiceStatusHandle, &ServiceStatus))
	{
		DWORD nError = GetLastError();
	}
}


void WINAPI service_main(int argc, char** argv)
{
	ServiceStatus.dwServiceType = SERVICE_WIN32;
	ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
	ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN | SERVICE_ACCEPT_PAUSE_CONTINUE;
	ServiceStatus.dwWin32ExitCode = 0;
	ServiceStatus.dwServiceSpecificExitCode = 0;
	ServiceStatus.dwCheckPoint = 0;
	ServiceStatus.dwWaitHint = 0;
	hServiceStatusHandle = RegisterServiceCtrlHandler(SERVICE_NAME, ServiceHandler);
	if (hServiceStatusHandle == 0)
	{
		DWORD nError = GetLastError();
	}
	//开启服务
	HANDLE task_handle = CreateThread(NULL, NULL, srv_core_thread, NULL, NULL, NULL);
	if (NULL == task_handle)
	{
		return;
	}
		
	// Initialization complete - report running status   
	ServiceStatus.dwCurrentState = SERVICE_RUNNING;
	ServiceStatus.dwCheckPoint = 0;
	ServiceStatus.dwWaitHint = 9000;
	if (!SetServiceStatus(hServiceStatusHandle, &ServiceStatus))
	{
		DWORD nError = GetLastError();
	}

}