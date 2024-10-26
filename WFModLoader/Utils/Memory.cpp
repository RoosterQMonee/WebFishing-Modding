#include "Memory.h"
#include "Logger.h"
#include "Misc.h"

#include <TlHelp32.h>


// Helper funcs


bool IsWFExecutable(const PROCESSENTRY32W& entry) {
    return std::wstring(entry.szExeFile) == L"webfishing.exe";
}

BOOL CALLBACK enumWindowsProc(HWND hwnd, LPARAM lParam) {
    const auto& pids = *reinterpret_cast<std::vector<DWORD>*>(lParam);

    DWORD winId;
    GetWindowThreadProcessId(hwnd, &winId);

    for (DWORD pid : pids) {
        if (winId == pid) {
            std::wstring title(GetWindowTextLength(hwnd) + 1, L'\0');
            GetWindowTextW(hwnd, &title[0], title.size());
        }
    }

    return TRUE;
}

void GetAllWindowsFromProcessID(DWORD dwProcessID, std::vector <HWND>& vhWnds)
{
    HWND hCurWnd = nullptr;
    do
    {
        hCurWnd = FindWindowEx(nullptr, hCurWnd, nullptr, nullptr);
        DWORD checkProcessID = 0;
        GetWindowThreadProcessId(hCurWnd, &checkProcessID);
        if (checkProcessID == dwProcessID)
            vhWnds.push_back(hCurWnd);

    } while (hCurWnd != nullptr);
}

void GetAllPartialWindows(std::vector<DWORD>& pids) {

    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    auto cleanupSnap = [snap] { CloseHandle(snap); };

    PROCESSENTRY32W entry;
    entry.dwSize = sizeof entry;

    if (!Process32FirstW(snap, &entry)) {
        cleanupSnap();
        Logger::Write<LogLevel::ERR>("Memory", "Cannot find webfishing.exe!");
        return;
    }

    do {
        if (IsWFExecutable(entry))
            pids.emplace_back(entry.th32ProcessID);
    } while (Process32NextW(snap, &entry));
    cleanupSnap();

    EnumWindows(enumWindowsProc, reinterpret_cast<LPARAM>(&pids));
}


// Memory Class


void Memory::Init(LPCSTR windowTitle, bool autoLaunch) {
    std::vector<DWORD> pids;
    std::vector<HWND> handles;

    GetAllPartialWindows(pids);

    if (pids.size() < 1) {
        Logger::Write<LogLevel::ERR>("Memory", "WebFishing isn't running!");

        if (autoLaunch) {
            long long ctime = Misc::CurrentMs();

            Logger::Write<LogLevel::INFO>("Memory", "Launching WebFishing...");
            system("start steam://launch/3146520");

            do {
                GetAllPartialWindows(pids);
                Sleep(1000);

                if ((Misc::CurrentMs() - ctime) > 10000) break;
            } while (pids.size() < 1);
            Sleep(5000); // just to give the game some time to load
        }
        else goto failed;
    }

    GetAllWindowsFromProcessID(pids.at(0), handles);
    Memory::gameHandle = handles.at(0);
    
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


bool Memory::LLInject(const char* filepath)
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