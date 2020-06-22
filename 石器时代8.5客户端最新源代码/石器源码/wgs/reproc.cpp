#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>
#include "../systeminc/version.h"
/*
HANDLE	hProcessSnap = NULL;
DWORD	dwPID;

void CloseParentProcess()
{
	PROCESSENTRY32 pe32;
	HANDLE	hParentProcess;
	dwPID = GetCurrentProcessId();
	if( INVALID_HANDLE_VALUE != (hProcessSnap=CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, NULL))){
		PROCESSENTRY32 pe32;
		pe32.dwSize = sizeof(PROCESSENTRY32);
		if( Process32First( hProcessSnap, &pe32)){ 
			do{
				if( pe32.th32ProcessID == dwPID){
					if( hParentProcess = OpenProcess( PROCESS_ALL_ACCESS, FALSE, pe32.th32ParentProcessID)){
//						MessageBoxNew( NULL, "找到了", "寻找父程式", MB_OK);
						TerminateProcess( hParentProcess, 0);
						CloseHandle( hParentProcess);
					}
					break;
				}
			}while( Process32Next( hProcessSnap, &pe32));
		}
		CloseHandle( hProcessSnap);
	}
}

*/