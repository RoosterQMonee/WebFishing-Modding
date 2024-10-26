#include "Loader.h"
#include <Utils/Logger.h>

#include <filesystem>


bool LLInject(const char* filepath, DWORD pid)
{
    HANDLE hProcess, hThread;
    SIZE_T bytesWritten;
    LPVOID memoryBuffer;

    hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

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

        return true;
    }
    else
    {
        Logger::Write<LogLevel::ERR>("Loader", "Cannot write to game!");
    }

    VirtualFreeEx(hProcess, NULL, strlen(filepath), MEM_RESERVE | MEM_COMMIT);
    CloseHandle(hProcess);

    return false;
}

void ModLoader::LoadMods(std::string path) {
    for (const std::filesystem::directory_entry& dir_entry : std::filesystem::recursive_directory_iterator(path))
    {
        Logger::Write<LogLevel::DEBUG>("Load", "Loading mod: ", dir_entry.path().filename().string());

        if (!LLInject(dir_entry.path().string().c_str(), GetCurrentProcessId()))
            Logger::Write<LogLevel::ERR>("Load", "Error while loading!");
    }
}