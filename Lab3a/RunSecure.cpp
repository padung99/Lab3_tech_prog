#include <iostream>
#include <windows.h>
#include <winreg.h>
#include <WinError.h>
#include <fstream>
#include <string>
#include <atlbase.h>
//#include <openssl/aes.h>
//#include <openssl/evp.h>
//#include <openssl/rsa.h>
//#include <openssl/pem.h>
//#include <openssl/ssl.h>
//#include <openssl/bio.h>
//#include <openssl/err.h>
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
    else if(wstring(processName2) == wstring(processName5))
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

int main()
{
    ShowWindow(GetConsoleWindow(), SW_HIDE); ///////////// Run in background
    HKEY hKey;
    LPCSTR sk_sys = "SOFTWARE\\Lab3Installer";
    LPCSTR value_InstPath = "InstallPath";
    LPCSTR value_sys = "SysInfor";
    char Link_InstPath[BUFFER];
    char Link[BUFFER];
    DWORD BufferSize_sys = BUFFER;
    DWORD BufferSize_Inst = BUFFER;

    RegGetValueA(HKEY_CURRENT_USER,  //Read path of sys.tat file form registry
        sk_sys,
        value_InstPath,
        RRF_RT_REG_SZ,
        NULL,
        (PVOID)&Link_InstPath,
        &BufferSize_sys);
    

    string str(Link_InstPath);
    string AppName = "\\VerifiedSignature.exe";
    string sum = str + AppName;
    LPCSTR Path = sum.c_str();
    int flag = 0;
    //string str(Link);

    RegGetValueA(HKEY_CURRENT_USER,  //Read path of sys.tat file form registry
        sk_sys,
        value_sys,
        RRF_RT_REG_SZ,
        NULL,
        (PVOID)&Link,
        &BufferSize_Inst);
    //system(Path);
    //TerminateProcessEx(FindProcessId(Link), 9);
    //cout << FindProcessId(Link) << endl;
    //DWORD flag1 = FindProcessId(Link);
    DWORD flag2 = 0;
    string cmd_name = "TurnOff.exe";
    const char* cstr = cmd_name.c_str();
    TerminateProcessEx(FindProcessId((char*)cstr), 9); 
    while (1)
    {
        //flag2 = FindProcessId(Link);
        if (FindProcessId(Link) != 0 && flag2 %2 == 0)
        {
            system(Path);
            flag2 = 1;
            //TerminateProcessEx(FindProcessId((char*)cstr), 9);
        }
        if (FindProcessId(Link) == 0)
            flag2 = 0;
    }

    return 0;
}