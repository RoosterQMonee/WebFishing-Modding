#include <WFCore/Core.h>
#include <WFCore/Config/Steam.h>

#include <Utils/Logger.h>
#include <Utils/Data.h>

#include <filesystem>
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

    Logger::Write<LogLevel::ERR>("Debug", "Exception thrown at 0x", exceptionAddr);

    return ExceptionContinueSearch;
}


void WFCore::Init() {
    SetUnhandledExceptionFilter(_UnhandledExceptionsHandler);
	WFCore::BuildVersion = Version{ 1, 0, 'a' };

	Logger::CreateConsole(std::format("WFCore v{}.{}{}", WFCore::BuildVersion.Major, WFCore::BuildVersion.Minor, WFCore::BuildVersion.Suffix).c_str());
	WFCore::Window = FindWindowA(nullptr, "Fish! (On the WEB!)");
	WFCore::WFDirectory = Steam::FindWFPath();

    if (WFCore::WFDirectory == "bad-install") {
        Logger::Write<LogLevel::ERR>("Debug", "Cannot locate Steam folder!");
        goto shutdown;
    } else if (WFCore::WFDirectory == "bad-library") {
        Logger::Write<LogLevel::ERR>("Debug", "Cannot locate Steam library!");
        goto shutdown;
    } else if (!std::filesystem::exists(WFCore::WFDirectory + "\\webfishing.exe")) {
        Logger::Write<LogLevel::ERR>("Debug", "Cannot locate WebFishing folder!");
        goto shutdown;
    }

	std::cout << WFCore::WFDirectory << std::endl;

shutdown:
	WFCore::Shutdown();
}

void WFCore::Shutdown() {
	FreeConsole();
	Sleep(1000);
	FreeLibraryAndExitThread(WFCore::ModuleHandle, 1);
}