#include "Memory.h"
#include "Logger.h"


void Memory::Init(LPCSTR windowTitle) {
    Memory::gameHandle = FindWindowA(windowTitle, NULL);
    
    if (!Memory::gameHandle) {
        Logger::Write<LogLevel::ERR>("Memory", "Cannot find ", windowTitle, "!");
        goto failed;
    }
    
    GetWindowThreadProcessId(Memory::gameHandle, &Memory::processID);

    if (!Memory::processID) {
        Logger::Write<LogLevel::ERR>("Memory", "Couldn't get ", windowTitle, " pid!");
        goto failed;
    }

failed:
    if (Memory::gameHandle != NULL)
        CloseHandle(Memory::gameHandle);
    return;
}


bool Memory::LLInject(char* filepath)
{
    HANDLE hProcess, hThread;
    SIZE_T bytesWritten;
    LPVOID memoryBuffer;

    hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, Memory::processID);

    if (!hProcess) {
        Logger::Write<LogLevel::ERR>("Loader", "Failed to get game handle!");
        return false;
    }

    memoryBuffer = VirtualAllocEx(hProcess, NULL, strlen(filepath), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

    if (memoryBuffer == NULL)
    {
        Logger::Write<LogLevel::ERR>("Loader", "Failed to allocate buffer!");
        CloseHandle(hProcess);
        return false;
    }

    if (WriteProcessMemory(hProcess, memoryBuffer, (LPVOID)filepath, strlen(filepath), &bytesWritten))
    {
        hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandleA("KERNEL32.DLL"), "LoadLibraryA"), memoryBuffer, 0, NULL);

        if (!hThread)
        {
            Logger::Write<LogLevel::ERR>("Loader", "Failed to create thread!");
            VirtualFreeEx(hProcess, NULL, strlen(filepath), MEM_RESERVE | MEM_COMMIT);
            CloseHandle(hProcess);
            return false;
        }

        VirtualFreeEx(hProcess, NULL, strlen(filepath), MEM_RESERVE | MEM_COMMIT);

        CloseHandle(hThread);
        CloseHandle(hProcess);

        return 1;
    }
    else
    {
        Logger::Write<LogLevel::ERR>("Loader", "Cannot write to game!");
    }

    VirtualFreeEx(hProcess, NULL, strlen(filepath), MEM_RESERVE | MEM_COMMIT);
    CloseHandle(hProcess);

    return false;
}