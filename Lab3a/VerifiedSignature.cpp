#include <iostream>
#include <windows.h>
#include <winreg.h>
#include <WinError.h>
#include <fstream>
#include <string>
#include <atlbase.h>
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <assert.h>
#include <TCHAR.H>
#include <tlhelp32.h>
#include <sddl.h>
#include <winuser.h>
#pragma warning(disable : 4996)
#define BUFFER 176

using namespace std;

//std::string publicKey = "-----BEGIN PUBLIC KEY-----\n"
//"MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQDn55DfFJA4MEyswSYVFNmG / X / W\n"
//"I86b / +KPyBscHA1N03sHyFe69du3fbVcA4geEqQqsoQj + 2sxiyfbg0lCfa7veyVv\n"
//"OO6vIUkqdetIKliw5w0pyx1G61PmVhNpUcOarVFxdf2zuxt4CkGdIHrDfzsOLk2L\n"
//"7Nv / Y2Q4C7C5wrHJfwIDAQAB\n"
//"-----END PUBLIC KEY-----";

std::string publicKey = "-----BEGIN PUBLIC KEY-----\n"
"MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQC0AC + BoVBSKWjrTcXogkAS0jmQ\n"
"k2fB1dg + 5d83ivIDHh3Yowteznxbd3Lka27u3ZqunEnIBFzeOKUxg8yM0L + bdjcl\n"
"1cT265PTxEG9vIjWfGbmOM63DnB7wS / eA3n + 1BBQkrEgFO97UcRyOp6x9izt02x1\n"
"bbsqqGxlNR + 7REdMFwIDAQAB\n"
"-----END PUBLIC KEY-----";

//std::string publicKey = "-----BEGIN PUBLIC KEY-----\n"\
//"MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAy8Dbv8prpJ/0kKhlGeJY\n"\
//"ozo2t60EG8L0561g13R29LvMR5hyvGZlGJpmn65+A4xHXInJYiPuKzrKUnApeLZ+\n"\
//"vw1HocOAZtWK0z3r26uA8kQYOKX9Qt/DbCdvsF9wF8gRK0ptx9M6R13NvBxvVQAp\n"\
//"fc9jB9nTzphOgM4JiEYvlV8FLhg9yZovMYd6Wwf3aoXK891VQxTr/kQYoq1Yp+68\n"\
//"i6T4nNq7NWC+UNVjQHxNQMQMzU6lWCX8zyg3yH88OAQkUXIXKfQ+NkvYQ1cxaMoV\n"\
//"PpY72+eVthKzpMeyHkBn7ciumk5qgLTEJAfWZpe4f4eFZj/Rc8Y8Jj2IS5kVPjUy\n"\
//"wQIDAQAB\n"\
//"-----END PUBLIC KEY-----\n";

BOOL CreateMyDACL(SECURITY_ATTRIBUTES* pSA)
{
    PULONG nSize = 0;

    const WCHAR* szSD = L"D:"           // Дискреционный ACL 
        L"(D;OICI;GA;;;BG)"           // Запрещаем доступ встроенным гостям 
        L"(D;OICI;GA;;;AN)"         // Запрещаем доступ к анонимному входу 
        L"(A;OICI;GR;;;AU)"           // Разрешить чтение для аутентификации 
        L"(A;OICI;GR;;;BA)";        // Разрешить чтение администратору 

    if (pSA == NULL)
        return FALSE;
    else
       // wprintf(L"SECURITY_ATTRIBUTES was passed...\n");

    // Выполняем некоторые проверки 

    //wprintf(L"The ACE strings : % s \n", szSD);
    //wprintf(L"The size : % d \n", pSA->nLength);
    //wprintf(L"The converted string is at : % p \n", &(pSA->lpSecurityDescriptor));

    // Преобразуем строку в двоичный дескриптор безопасности и вернем 
    return ConvertStringSecurityDescriptorToSecurityDescriptor(
        szSD,                         // Строки ACE 
        SDDL_REVISION_1,              // Стандартный уровень ревизии 
        &(pSA->lpSecurityDescriptor), // Указатель на преобразованный дескриптор безопасности 
        nSize);                // Размер в байтах преобразованного дескриптора безопасности 
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

DWORD FindProcessId(char* processName) //Check if file is opening
{
    // strip path
    char* p = strrchr(processName, '\\');
    if (p)
        processName = p + 1;
    
    const size_t cSize = strlen(processName) + 1;
    const size_t cSize2 = strlen("sys.tat") + 1;
    const size_t cSize3 = strlen("notepad++.exe") + 1;

    wchar_t* processName2 = new wchar_t[cSize];
    wchar_t* processName3 = new wchar_t[cSize2];
    wchar_t* processName4 = new wchar_t[cSize3];

    mbstowcs(processName2, processName, cSize);
    mbstowcs(processName3, (char*)"sys.tat", cSize2);
    mbstowcs(processName4, (char*)"notepad++.exe", cSize3);

    if (wstring(processName2) == wstring(processName3))
        processName2 = processName4;
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

RSA* createPublicRSA(std::string key) {
    RSA* rsa = NULL;
    BIO* keybio;
    const char* c_string = key.c_str();
    keybio = BIO_new_mem_buf((void*)c_string, -1);
    if (keybio == NULL) {
        return 0;
    }
    rsa = PEM_read_bio_RSA_PUBKEY(keybio, &rsa, NULL, NULL);
    return rsa;
}

bool RSAVerifySignature(RSA* rsa,
    unsigned char* MsgHash,
    size_t MsgHashLen,
    const char* Msg,
    size_t MsgLen,
    bool* Authentic) {
    *Authentic = false;
    EVP_PKEY* pubKey = EVP_PKEY_new();
    EVP_PKEY_assign_RSA(pubKey, rsa);
    EVP_MD_CTX* m_RSAVerifyCtx = EVP_MD_CTX_create();

    if (EVP_DigestVerifyInit(m_RSAVerifyCtx, NULL, EVP_sha256(), NULL, pubKey) <= 0) {
        return false;
    }
    if (EVP_DigestVerifyUpdate(m_RSAVerifyCtx, Msg, MsgLen) <= 0) {
        return false;
    }
    int AuthStatus = EVP_DigestVerifyFinal(m_RSAVerifyCtx, MsgHash, MsgHashLen);
    if (AuthStatus == 1) {
        *Authentic = true;
        EVP_MD_CTX_free(m_RSAVerifyCtx);
        return true;
    }
    else if (AuthStatus == 0) {
        *Authentic = false;
        EVP_MD_CTX_free(m_RSAVerifyCtx);
        return true;
    }
    else {
        *Authentic = false;
        EVP_MD_CTX_free(m_RSAVerifyCtx);
        return false;
    }
}

size_t calcDecodeLength(const char* b64input) {
    size_t len = strlen(b64input), padding = 0;

    if (b64input[len - 1] == '=' && b64input[len - 2] == '=') //last two chars are =
        padding = 2;
    else if (b64input[len - 1] == '=') //last char is =
        padding = 1;
    return (len * 3) / 4 - padding;
}


void Base64Decode(const char* b64message, unsigned char** buffer, size_t* length) {
    BIO* bio, * b64;

    int decodeLen = calcDecodeLength(b64message);
    *buffer = (unsigned char*)malloc(decodeLen + 1);
    (*buffer)[decodeLen] = '\0';

    bio = BIO_new_mem_buf(b64message, -1);
    b64 = BIO_new(BIO_f_base64());
    bio = BIO_push(b64, bio);

    *length = BIO_read(bio, *buffer, strlen(b64message));
    BIO_free_all(bio);
}

bool verifySignature(std::string publicKey, std::string plainText, char* signatureBase64) {
    RSA* publicRSA = createPublicRSA(publicKey);
    unsigned char* encMessage;
    size_t encMessageLength;
    bool authentic;
    Base64Decode(signatureBase64, &encMessage, &encMessageLength);
    bool result = RSAVerifySignature(publicRSA, encMessage, encMessageLength, plainText.c_str(), plainText.length(), &authentic);
    return result & authentic;
}

int main() {
    //std::string plainText = "My secret message.\n";
    //char* signature = signMessage(privateKey, plainText);
    ShowWindow(GetConsoleWindow(), SW_HIDE);////////////////////////////////////
    HKEY hKey; 
    LPCSTR sk_sys = "SOFTWARE\\Lab3Installer";
    LPCSTR value_Name = "StudentName";
    char Name[BUFFER];
    DWORD BufferSize_name = BUFFER;

    RegGetValueA(HKEY_CURRENT_USER,  //Read name of student from registry
        sk_sys,
        value_Name,
        RRF_RT_REG_SZ,
        NULL,
        (PVOID)&Name,
        &BufferSize_name);
    string str1(Name);

    LPCSTR value_sys = "SysInfor";
    char Link[BUFFER];
    DWORD BufferSize_sys = BUFFER;

    RegGetValueA(HKEY_CURRENT_USER,  //Read path of sys.tat file form registry
        sk_sys,
        value_sys,
        RRF_RT_REG_SZ,
        NULL,
        (PVOID)&Link,
        &BufferSize_sys);
    string str(Link), Info;

    //Deny access file to unauthority user (Lab1)
    SECURITY_ATTRIBUTES  sa;
    BOOL RetVal;

    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.bInheritHandle = FALSE;
    
    RetVal = CreateMyDACL(&sa);
    if (!RetVal)
    {
         
        wprintf(L"Failed CreateMyDACL() for reset, error % d\n", GetLastError());
        exit(1);
    }
    else
        wprintf(L"CreateMyDACL() for reset is OK! Returned value is % d\n", RetVal);
    
    SetFileSecurity(  
        (LPCWSTR)Link,
        DACL_SECURITY_INFORMATION,
        sa.lpSecurityDescriptor
    );


    string tmp = "SOFTWARE\\";

    string sk_tmp = tmp + str1; //Sum 2 strings
    LPCSTR sk = sk_tmp.c_str(); //Convert from string to LPCSTR 
    LPCSTR value = "Signature";
    char Sign[BUFFER]; //Read signature from registry
    DWORD BufferSize = BUFFER;

    RegGetValueA(HKEY_CURRENT_USER,
        sk,
        value,
        RRF_RT_REG_MULTI_SZ,
        NULL,
        (PVOID)&Sign,
        &BufferSize);

    ///// another way to open application - Use CreateProcess() function
    //system("C:\\Users\\dungphan16499\\Desktop\\Lab3Installer\\RunSecure.exe");
    //HANDLE hProcess;
    //HANDLE hThread;
    //STARTUPINFO si;
    //PROCESS_INFORMATION pi;
    //DWORD dwProcessId = 0;
    //DWORD dwThreadId = 0;
    //ZeroMemory(&si, sizeof(si));
    //ZeroMemory(&pi, sizeof(pi));
    //BOOL bCreateProcess = NULL;
    //bCreateProcess = CreateProcess(
    //    L"C:\\Program Files (x86)\\Notepad++\\notepad++.exe",
    //    (LPWSTR)"C:\\Users\\dungphan16499\\Desktop\\Lab3Installer\\sys.tat",
    //    NULL,
    //    NULL,
    //    FALSE,
    //    0,
    //    NULL,
    //    NULL,
    //    &si,
    //    &pi); 
    //if (bCreateProcess == FALSE)
    //    cout << "Create process fail & Error: " << GetLastError() << endl;
    //else
    //    cout << "Create Process successfully" << endl;
    //cout << "Process ID ->" << pi.dwProcessId << endl;
    //cout << "Thread ID -> " << pi.dwThreadId << endl;
    //cout << "GetProcessID ->" << GetProcessId(pi.hProcess) << endl;
    //cout << "GetThreadID -> " << GetThreadId(pi.hThread) << endl;
    //WaitForSingleObject(pi.hProcess, INFINITE);
    //CloseHandle(pi.hThread);
    //CloseHandle(pi.hProcess);

    { 
    ifstream myfile;
    myfile.open(str);

    if (myfile.is_open()) //Read data from sys.tat file (Hashed)
    {
        while (getline(myfile, Info))
        {
            cout << Info << endl;
        }
        myfile.close();
    }
    else
        cout << "Unable to open file";

    LPCSTR value_InstPath = "InstallPath";
    char Link_InstPath[BUFFER];
    DWORD BufferSize_Inst = BUFFER;

    RegGetValueA(HKEY_CURRENT_USER,  //Read path of Message.exe form registry
        sk_sys,
        value_InstPath,
        RRF_RT_REG_SZ,
        NULL,
        (PVOID)&Link_InstPath,
        &BufferSize_sys);

    string str_inst(Link_InstPath);
    string AppName = "\\Message.exe";
    string sum_inst = str_inst + AppName;
    LPCSTR Path_inst = sum_inst.c_str();

    bool authentic = verifySignature(publicKey, Info, Sign);
    if (authentic) {
        std::cout << "Authentic" << std::endl;
        cout << Sign << endl;
        RegOpenKeyExA(HKEY_CURRENT_USER, sk_sys, 0, KEY_ALL_ACCESS, &hKey);
        RegSetValueExA(hKey, "Status", 0, REG_SZ, (LPBYTE)"Authentic", BUFFER);
    }
    else { 
        cout << "Non authentic" << endl;
        cout << Sign << endl;
        RegOpenKeyExA(HKEY_CURRENT_USER, sk_sys, 0, KEY_ALL_ACCESS, &hKey);
        RegSetValueExA(hKey, "Status", 0, REG_SZ, (LPBYTE)"Not Authentic", BUFFER);
        TerminateProcessEx(FindProcessId(Link), 9);
        system(Path_inst);
        
        //ShowWindow(GetConsoleWindow(), SW_SHOWNORMAL);
        //char button;
        //cout << "Press 'y' button to continue" << endl;
        //cin >> button;

        //while(button != 'y')
        //{
        //    cout << "Invalid button" << endl;
        //    cout << "Press 'y' button to continue" << endl;
        //    cin >> button;
        //}
        //if (button == 'y')
        //    ShowWindow(GetConsoleWindow(), SW_HIDE);
    }
    }
}