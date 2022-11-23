#include <iostream>
#include <windows.h>
#include <process.h>
#include <Tlhelp32.h>
#include <winbase.h>
#include <cstring>
#include <string>
#include <vector>
#include <sstream>

const std::string ENV = "PROC_TO_KILL";
const std::string PROC_TO_KILL = "explorer.exe";

//void kill_process_by_id(int id){
//    HANDLE descriptor = OpenProcess(PROCESS_TERMINATE, FALSE, id);
//    if (descriptor != nullptr) {
//        TerminateProcess(descriptor, 0);
//        CloseHandle(descriptor);
//    }
//}

void kill_process_by_name(const char* name) {
    HANDLE snapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);
    PROCESSENTRY32 procEntry;
    procEntry.dwSize = sizeof(procEntry);
    BOOL isProc = Process32First(snapShot, &procEntry);
    while (isProc) {
        if (strcmp(procEntry.szExeFile, name) == 0) {
            HANDLE procDescrptr = OpenProcess(PROCESS_TERMINATE, 0,
                                              (DWORD) procEntry.th32ProcessID);
            if (procDescrptr != nullptr) {
                TerminateProcess(procDescrptr, 9);
                CloseHandle(procDescrptr);
            }
        }
        isProc = Process32Next(snapShot, &procEntry);
    }
    CloseHandle(snapShot);
}

void killEnviromentVariable() {
    const DWORD max_size = 256;
    char buff[max_size];
    if(!GetEnvironmentVariable(ENV.c_str(), buff, max_size)) {
        return;
    }

    std::stringstream ss(buff);
    while (ss.getline(buff, max_size, ',')) {
        kill_process_by_name(buff);
    }
}

int main()
{
    SetEnvironmentVariableA(ENV.c_str(), PROC_TO_KILL.c_str());
    killEnviromentVariable();
    return 0;
}
