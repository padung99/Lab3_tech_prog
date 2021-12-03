#include <iostream>
#include <windows.h>
#include <winreg.h>
#include <WinError.h>
#include <fstream>
#include <string>
#include <atlbase.h>
#include <assert.h>
#include <TCHAR.H>
#include <tlhelp32.h>
#include <sddl.h>
#pragma warning(disable : 4996)
#define BUFFER 176

using namespace std;

DWORD FindProcessId(char* processName) //Check if file is opening
{
    // strip path
    char* p = strrchr(processName, '\\');
    if (p)
        processName = p + 1;

    const size_t cSize = strlen(processName) + 1;
    const size_t cSize2 = strlen("sys.tat") + 1;
    const size_t cSize3 = strlen("notepad++.exe") + 1;
    const size_t cSize4 = strlen("TurnOff.exe") + 1;

    wchar_t* processName2 = new wchar_t[cSize];
    wchar_t* processName3 = new wchar_t[cSize2];
    wchar_t* processName4 = new wchar_t[cSize3];
    wchar_t* processName5 = new wchar_t[cSize4];

    mbstowcs(processName2, processName, cSize);
    mbstowcs(processName3, (char*)"sys.tat", cSize2);
    mbstowcs(processName4, (char*)"notepad++.exe", cSize3);
    mbstowcs(processName5, (char*)"TurnOff.exe", cSize4);

    if (wstring(processName2) == wstring(processName3))
        processName2 = processName4;
    else if (wstring(processName2) == wstring(processName5))
        processName2 = processName5;
    else
        return 0;

    PROCESSENTRY32 processInfo;
    processInfo.dwSize = sizeof(processInfo);
    HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    if (processesSnapshot == INVALID_HANDLE_VALUE)
        return 0;
    Process32First(processesSnapshot, &processInfo);

    if (!wcscmp(processName2, processInfo.szExeFile))
    {
        CloseHandle(processesSnapshot);
        return processInfo.th32ProcessID;
    }
    while (Process32Next(processesSnapshot, &processInfo))
    {
        if (!wcscmp(processName2, processInfo.szExeFile))
        {
            CloseHandle(processesSnapshot);
            return processInfo.th32ProcessID;
        }
    }
    CloseHandle(processesSnapshot);
    return 0;
}

BOOL TerminateProcessEx(DWORD dwProcessId, UINT uExitCode)  //Force closing file sys.tat
{
    DWORD dwDesiredAccess = PROCESS_TERMINATE;
    BOOL  bInheritHandle = FALSE;
    HANDLE hProcess = OpenProcess(dwDesiredAccess, bInheritHandle, dwProcessId);
    if (hProcess == NULL)
        return FALSE;

    BOOL result = TerminateProcess(hProcess, uExitCode);

    CloseHandle(hProcess);

    return result;
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow){
	//LPCWSTR mess = L"FILE HAS BEEN CHANGED BY NON AUTHORITY USER\n"
	//	"THIS FILE WILL NOT ABLE TO WIEW ANYMORE\n"
	//	"PLEASE CONTACT WITH AUTHOR";
	//System.Windows.Forms.DialogResult dlgResult dresult = MessageBox.Show("Are you sure ", "Alert"
	//	, MessageBoxButtons.OKCancel, MessageBoxIcon.Question);

	//MessageBox(NULL, mess, L"Warning", MB_OK);

	LPCWSTR mess = L"FILE HAS BEEN CHANGED BY NON AUTHORITY USER\n"
			"THIS FILE WILL NOT ABLE TO WIEW ANYMORE\n"
			"PLEASE CONTACT WITH AUTHOR";

	const int result = MessageBox(NULL, mess, L"Warning", MB_OK);

    //string cmd_name = "notepad++.exe";
    //const char* cstr = cmd_name.c_str();
    
	//if (result == IDOK)
	//	MessageBox(NULL, L"OK", L"Checking", MB_OK);
    while (result != IDOK || result != IDCANCEL)

	return 0;
}