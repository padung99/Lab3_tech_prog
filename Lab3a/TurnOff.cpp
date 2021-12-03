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

int main()
{
	HKEY hKey;
	LPCSTR sk_sys = "SOFTWARE\\Lab3Installer";
	LPCSTR value_InstPath = "InstallPath";
	char Link_InstPath[BUFFER];
	DWORD BufferSize_Inst = BUFFER;

	RegGetValueA(HKEY_CURRENT_USER,  //Read path of sys.tat file form registry
		sk_sys,
		value_InstPath,
		RRF_RT_REG_SZ,
		NULL,
		(PVOID)&Link_InstPath,
		&BufferSize_Inst);

	string str(Link_InstPath);
	string AppName = "\\RunSecure.exe";
	string sum = str + AppName;
	LPCSTR Path = sum.c_str();

	system(Path);
}