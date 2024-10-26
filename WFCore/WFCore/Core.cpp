#include <WFCore/Core.h>
#include <WFCore/Config/Steam.h>

#include <Utils/Logger.h>
#include <Utils/Data.h>

#include <TlHelp32.h>
#include <Psapi.h>
#include <format>
#include <cstdio>


LONG WINAPI _UnhandledExceptionsHandler(EXCEPTION_POINTERS* ExceptionInfo)
{
    if (ExceptionInfo == nullptr) {
        std::cerr << "ExceptionInfo is nullptr" << std::endl;
        return ExceptionContinueSearch;
    }

    HMODULE hModule = GetModuleHandle(nullptr);

    TCHAR szModuleName[MAX_PATH];
    GetModuleFileName(hModule, szModuleName, MAX_PATH);

    EXCEPTION_RECORD* exceptionRecord = ExceptionInfo->ExceptionRecord;
    uint64_t exceptionAddr = (uint64_t)exceptionRecord->ExceptionAddress;

    Logger::Write<LogLevel::ERR>("Debug", "Exception thrown at 0x", exceptionAddr, "\n\t> ", szModuleName);

    return ExceptionContinueSearch;
}


void WFCore::Init() {
    SetUnhandledExceptionFilter(_UnhandledExceptionsHandler);
	WFCore::BuildVersion = Version{ 1, 0, 'a' };

	Logger::CreateConsole(std::format("WFCore v{}.{}{}", WFCore::BuildVersion.Major, WFCore::BuildVersion.Minor, WFCore::BuildVersion.Suffix).c_str());
	WFCore::Window = FindWindowA(nullptr, "Fish! (On the WEB!)");
	WFCore::WFDirectory = Steam::FindWFPath();

	std::cout << WFCore::WFDirectory << std::endl;

shutdown:
	WFCore::Shutdown();
}

void WFCore::Shutdown() {
	FreeConsole();
	Sleep(1000);
	FreeLibraryAndExitThread(WFCore::ModuleHandle, 1);
}