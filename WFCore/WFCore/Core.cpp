#include "CoreExt.h"

#include <WFCore/Core.h>
#include <WFCore/Config/Steam.h>
#include <WFCore/Config/Files.h>

#include <Utils/Data.h>
#include <Utils/Logger.h>

#include <WFCore/Mods/Exports.h>
#include <WFCore/Mods/Loader.h>


void WFCore::Init() {
    SetUnhandledExceptionFilter(_UnhandledExceptionsHandler);
    WFCore::BuildVersion = Version{ 1, 0, 'a' };

    Logger::CreateConsole(std::format("WFCore v{}.{}{}", WFCore::BuildVersion.Major, WFCore::BuildVersion.Minor, WFCore::BuildVersion.Suffix).c_str());
    WFCore::Window = FindWindowA(nullptr, "Fish! (On the WEB!)");
    WFCore::WFDirectory = Steam::FindWFPath();

    if (WFCore::WFDirectory == "bad-install") {
        Logger::Write<LogLevel::ERR>("Debug", "Cannot locate Steam folder! Please select the correct folder.");
        ManualSearch("C:/");
    }
    else if (WFCore::WFDirectory == "bad-library") {
        Logger::Write<LogLevel::ERR>("Debug", "Cannot locate Steam library! Please select the correct folder.");
        ManualSearch("C:/");
    }
    else if (!std::filesystem::exists(WFCore::WFDirectory + "\\webfishing.exe")) {
        Logger::Write<LogLevel::ERR>("Debug", "Cannot locate WebFishing folder! Please select the correct folder.");
        ManualSearch(WFCore::WFDirectory != "" ? WFCore::WFDirectory : "C:/");
    }

    Files::CheckFileStructure(WFCore::WFDirectory);
    ModLoader::LoadMods(WFCore::WFDirectory + "\\wfmodder\\mods");

    Logger::Write<LogLevel::INFO>("WFCore", "Found WebFishing folder at ", WFCore::WFDirectory);

shutdown:
    while (true) { Sleep(50); }
    WFCore::Shutdown();
}

void WFCore::Shutdown() {
    FreeConsole();
    Sleep(1000);
    FreeLibraryAndExitThread(WFCore::ModuleHandle, 1);
}