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
#pragma warning(disable : 4996)
#define BUFFER 176

using namespace std;

//std::string privateKey = "-----BEGIN RSA PRIVATE KEY-----\n"
//"MIICXwIBAAKBgQDn55DfFJA4MEyswSYVFNmG / X / WI86b / +KPyBscHA1N03sHyFe6\n"
//"9du3fbVcA4geEqQqsoQj + 2sxiyfbg0lCfa7veyVvOO6vIUkqdetIKliw5w0pyx1G\n"
//"61PmVhNpUcOarVFxdf2zuxt4CkGdIHrDfzsOLk2L7Nv / Y2Q4C7C5wrHJfwIDAQAB\n"
//"AoGBAKdvpEPM / wvyo4mfszVJZn12jlQ4ajZsHFKfsiMrhdeJN604007jsI3dL5KN\n"
//"4juETg + d8JZiMtUEmfR95 + kgAar7MIjWMJZAGDG5F5DQqM4tSWpxDTw64nMdWU8J\n"
//"9TGI6FrLJouf7vdJwgG5EmhWMAuiv7A1I2Q + RyE0ISzU7i / hAkEA8 / oOyMoAkRiF\n"
//"PuceBc3yM3UI9vDE7WvKHpinW3xDPAi9yyKp70KjWqowV1XxLAKrx1uiUe7yeqnj\n"
//"CRL61IGXOQJBAPNVNVCYFwfUL5yc5 + 5z64 / 8iwdQFtNsDeQ / JVy8YLnTGLB8Xcgc\n"
//"WrbgoLfD + 223pOZpu4cEwuNzbcKv6z1KbncCQQCWyhEDJl4tT0NjOkLDAaNVDFuL\n"
//"DAoaV0lnmZ + uGmIPo7Nx7i2rU7tUmOUhxVs2gTwcDdWH4C / 7gi5w + EsQtjt5AkEA\n"
//"4o / HoE67wtJRsmR1npPYFKxzVk + / sQK + vHH++6V5 + gsOxeRF / XhJDlV / zxpvE + 0V\n"
//"YGO58zD / tH9ZBxh80whmaQJBAOdinVfmQ40aErwu4bwpN2JxpDQVRVMiFClAHIGJ\n"
//"xgSjxZ0 / bREokV / x9uDIDjBUM32gS8p63xofQYTqsQh8xLg =\n"
//"-----END RSA PRIVATE KEY-----";

std::string privateKey = "-----BEGIN RSA PRIVATE KEY-----\n"  //1024 bits key
"MIICXAIBAAKBgQC0AC + BoVBSKWjrTcXogkAS0jmQk2fB1dg + 5d83ivIDHh3Yowte\n"
"znxbd3Lka27u3ZqunEnIBFzeOKUxg8yM0L + bdjcl1cT265PTxEG9vIjWfGbmOM63\n"
"DnB7wS / eA3n + 1BBQkrEgFO97UcRyOp6x9izt02x1bbsqqGxlNR + 7REdMFwIDAQAB\n"
"AoGAZsooVt140cKGAMFkABfJYTYd21riCIYGvDxnEP / Onksnfhv1lUlTKKR4vso / \n"
"CjcQayAi3g7nvZsZcsIyIEPg9n80UGoiIOxM7WO + WlwCpe6o58lWOChxsGFqR7rv\n"
"AlZXiuWmtSbRUhjCBeypSwflpgljm1qJ9MmR1pqKhUtrV4ECQQDpKMxvW1QtLgU7\n"
"GyghL50PTxz2kEapH0ykUKWp5yVaDzbj2MXonTV + LoOpYSascUgkgM2CfOd / KZ5g\n"
"7 + kvKjVXAkEAxaJEeq1HjxkDJ2knacbHm4T7f9yZIeESarQovR98Nn9rXLx + K5fT\n"
"dJKKkEnBl5zPtcpp4f / xzinlq1zdjdynQQJAM6 + Xohfc0ScigbfDbZfgeURQZ1gU\n"
"UeleVA6Mqt1SqmQ1Bs1HEgnHkfE9vo6KxC9Yjr7 + z65gow / WzCZiytHQgQJAIABq\n"
"fkLYt38hV5xbWXFqmPArmUSrNyUuykBp6FXU0y5GwDl1ZLGs989SyJJo2pf1ukYQ\n"
"Zxv + l5eJLPpYlcQJAQJBAJd7of43SLhCko10clkPkofVsTseg0lB9IpkUCkvKfoJ\n"
"5bkCtYLnwHpyzMkh4Kd0FohcahafhM43e0ciWCHRoe4 = \n"
"-----END RSA PRIVATE KEY-----";

//std::string privateKey = "-----BEGIN RSA PRIVATE KEY-----\n"\
//"MIIEowIBAAKCAQEAy8Dbv8prpJ/0kKhlGeJYozo2t60EG8L0561g13R29LvMR5hy\n"\
//"vGZlGJpmn65+A4xHXInJYiPuKzrKUnApeLZ+vw1HocOAZtWK0z3r26uA8kQYOKX9\n"\
//"Qt/DbCdvsF9wF8gRK0ptx9M6R13NvBxvVQApfc9jB9nTzphOgM4JiEYvlV8FLhg9\n"\
//"yZovMYd6Wwf3aoXK891VQxTr/kQYoq1Yp+68i6T4nNq7NWC+UNVjQHxNQMQMzU6l\n"\
//"WCX8zyg3yH88OAQkUXIXKfQ+NkvYQ1cxaMoVPpY72+eVthKzpMeyHkBn7ciumk5q\n"\
//"gLTEJAfWZpe4f4eFZj/Rc8Y8Jj2IS5kVPjUywQIDAQABAoIBADhg1u1Mv1hAAlX8\n"\
//"omz1Gn2f4AAW2aos2cM5UDCNw1SYmj+9SRIkaxjRsE/C4o9sw1oxrg1/z6kajV0e\n"\
//"N/t008FdlVKHXAIYWF93JMoVvIpMmT8jft6AN/y3NMpivgt2inmmEJZYNioFJKZG\n"\
//"X+/vKYvsVISZm2fw8NfnKvAQK55yu+GRWBZGOeS9K+LbYvOwcrjKhHz66m4bedKd\n"\
//"gVAix6NE5iwmjNXktSQlJMCjbtdNXg/xo1/G4kG2p/MO1HLcKfe1N5FgBiXj3Qjl\n"\
//"vgvjJZkh1as2KTgaPOBqZaP03738VnYg23ISyvfT/teArVGtxrmFP7939EvJFKpF\n"\
//"1wTxuDkCgYEA7t0DR37zt+dEJy+5vm7zSmN97VenwQJFWMiulkHGa0yU3lLasxxu\n"\
//"m0oUtndIjenIvSx6t3Y+agK2F3EPbb0AZ5wZ1p1IXs4vktgeQwSSBdqcM8LZFDvZ\n"\
//"uPboQnJoRdIkd62XnP5ekIEIBAfOp8v2wFpSfE7nNH2u4CpAXNSF9HsCgYEA2l8D\n"\
//"JrDE5m9Kkn+J4l+AdGfeBL1igPF3DnuPoV67BpgiaAgI4h25UJzXiDKKoa706S0D\n"\
//"4XB74zOLX11MaGPMIdhlG+SgeQfNoC5lE4ZWXNyESJH1SVgRGT9nBC2vtL6bxCVV\n"\
//"WBkTeC5D6c/QXcai6yw6OYyNNdp0uznKURe1xvMCgYBVYYcEjWqMuAvyferFGV+5\n"\
//"nWqr5gM+yJMFM2bEqupD/HHSLoeiMm2O8KIKvwSeRYzNohKTdZ7FwgZYxr8fGMoG\n"\
//"PxQ1VK9DxCvZL4tRpVaU5Rmknud9hg9DQG6xIbgIDR+f79sb8QjYWmcFGc1SyWOA\n"\
//"SkjlykZ2yt4xnqi3BfiD9QKBgGqLgRYXmXp1QoVIBRaWUi55nzHg1XbkWZqPXvz1\n"\
//"I3uMLv1jLjJlHk3euKqTPmC05HoApKwSHeA0/gOBmg404xyAYJTDcCidTg6hlF96\n"\
//"ZBja3xApZuxqM62F6dV4FQqzFX0WWhWp5n301N33r0qR6FumMKJzmVJ1TA8tmzEF\n"\
//"yINRAoGBAJqioYs8rK6eXzA8ywYLjqTLu/yQSLBn/4ta36K8DyCoLNlNxSuox+A5\n"\
//"w6z2vEfRVQDq4Hm4vBzjdi3QfYLNkTiTqLcvgWZ+eX44ogXtdTDO7c+GeMKWz4XX\n"\
//"uJSUVL5+CVjKLjZEJ6Qc2WZLl94xSwL71E41H4YciVnSCQxVc4Jw\n"\
//"-----END RSA PRIVATE KEY-----\n\0";
//
//std::string publicKey = "-----BEGIN PUBLIC KEY-----\n"\
//"MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAy8Dbv8prpJ/0kKhlGeJY\n"\
//"ozo2t60EG8L0561g13R29LvMR5hyvGZlGJpmn65+A4xHXInJYiPuKzrKUnApeLZ+\n"\
//"vw1HocOAZtWK0z3r26uA8kQYOKX9Qt/DbCdvsF9wF8gRK0ptx9M6R13NvBxvVQAp\n"\
//"fc9jB9nTzphOgM4JiEYvlV8FLhg9yZovMYd6Wwf3aoXK891VQxTr/kQYoq1Yp+68\n"\
//"i6T4nNq7NWC+UNVjQHxNQMQMzU6lWCX8zyg3yH88OAQkUXIXKfQ+NkvYQ1cxaMoV\n"\
//"PpY72+eVthKzpMeyHkBn7ciumk5qgLTEJAfWZpe4f4eFZj/Rc8Y8Jj2IS5kVPjUy\n"\
//"wQIDAQAB\n"\
//"-----END PUBLIC KEY-----\n";



RSA* createPrivateRSA(std::string key) {
    RSA* rsa = NULL;
    const char* c_string = key.c_str();
    BIO* keybio = BIO_new_mem_buf((void*)c_string, -1);
    if (keybio == NULL) {
        return 0;
    }
    rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa, NULL, NULL);
    return rsa;
}


bool RSASign(RSA* rsa,
    const unsigned char* Msg,
    size_t MsgLen,
    unsigned char** EncMsg,
    size_t* MsgLenEnc) {
    EVP_MD_CTX* m_RSASignCtx = EVP_MD_CTX_create();
    EVP_PKEY* priKey = EVP_PKEY_new();
    EVP_PKEY_assign_RSA(priKey, rsa);
    if (EVP_DigestSignInit(m_RSASignCtx, NULL, EVP_sha256(), NULL, priKey) <= 0) {
        return false;
    }
    if (EVP_DigestSignUpdate(m_RSASignCtx, Msg, MsgLen) <= 0) {
        return false;
    }
    if (EVP_DigestSignFinal(m_RSASignCtx, NULL, MsgLenEnc) <= 0) {
        return false;
    }
    *EncMsg = (unsigned char*)malloc(*MsgLenEnc);
    if (EVP_DigestSignFinal(m_RSASignCtx, *EncMsg, MsgLenEnc) <= 0) {
        return false;
    }
    EVP_MD_CTX_free(m_RSASignCtx);
    return true;
}

void Base64Encode(const unsigned char* buffer,
    size_t length,
    char** base64Text) {
    BIO* bio, * b64;
    BUF_MEM* bufferPtr;

    b64 = BIO_new(BIO_f_base64());
    bio = BIO_new(BIO_s_mem());
    bio = BIO_push(b64, bio);

    BIO_write(bio, buffer, length);
    BIO_flush(bio);
    BIO_get_mem_ptr(bio, &bufferPtr);
    BIO_set_close(bio, BIO_NOCLOSE);
    BIO_free_all(bio);

    *base64Text = (*bufferPtr).data;
}


char* signMessage(std::string privateKey, std::string plainText) {
    RSA* privateRSA = createPrivateRSA(privateKey);
    unsigned char* encMessage;
    char* base64Text;
    size_t encMessageLength;
    RSASign(privateRSA, (unsigned char*)plainText.c_str(), plainText.length(), &encMessage, &encMessageLength);
    Base64Encode(encMessage, encMessageLength, &base64Text);
    free(encMessage);
    return base64Text;
}

int main() {
    
    ifstream myfile;
    LPCSTR sk_sys = "SOFTWARE\\Lab3Installer";
    LPCSTR value_sys = "SysInfor";
    char Link[BUFFER];
    DWORD BufferSize = BUFFER;
     
    RegGetValueA(HKEY_CURRENT_USER,  //Read path of sys.tat file
        sk_sys,
        value_sys,
        RRF_RT_REG_SZ,
        NULL,
        (PVOID)&Link,
        &BufferSize);
    string str(Link), Info;

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
        cout << "Unable to open file\n";

    std::string plainText = Info; //////////
    char *signature = signMessage(privateKey, plainText); 
  
    cout << signature << endl;

    string RegName; //Input student's name
    cout << "Name of registry: ";
    cin >> RegName;

    HKEY hKey, hKey1;  
    string tmp = "SOFTWARE\\";
    string sum = tmp + RegName; //Sum 2 strings 
    LPCSTR sk = sum.c_str();  //Convert from string to LPCSTR 

        long sts = RegCreateKeyExA(HKEY_CURRENT_USER, sk, 0L, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);
        if (sts == ERROR_SUCCESS)
            cout << "Successfully created new key" << endl;
        else
            cout << "Fail to create new key" << endl;

    RegOpenKeyExA(HKEY_CURRENT_USER, sk_sys, 0, KEY_ALL_ACCESS, &hKey1);
    RegSetValueExA(hKey1, "StudentName", 0, REG_SZ, (LPBYTE)RegName.c_str(), BUFFER);

    LONG openRes = RegOpenKeyExA(HKEY_CURRENT_USER, sk, 0, KEY_ALL_ACCESS, &hKey); ///////////
   
    if (openRes == ERROR_SUCCESS) {
        printf("Success opening key.\n");
    }
    else {
        printf("Error opening key.\n");
    }

    LPCSTR value = "Signature"; //Name of registry 
    //LPCSTR data = "TestData\0\0";
    LONG setRes = RegSetValueExA(hKey, value, 0, REG_MULTI_SZ, (LPBYTE)signature, BUFFER);/////////////////

    if (setRes == ERROR_SUCCESS) {
        printf("Success writing to Registry.\n");
    }
    else {
        printf("Error writing to Registry.\n");
    }

    LONG closeOut = RegCloseKey(hKey);

    if (closeOut == ERROR_SUCCESS) {
        printf("Success closing key.\n");
    }
    else {
        printf("Error closing key.\n");
    }
    system("Pause");
}