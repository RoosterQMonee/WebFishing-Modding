#include "Mod.h"

#include <Utils/Logger.h>
#include <SDK/SDK.h>


void Mod::Init() {
    Logger::CreateConsole();
    Logger::Write<LogLevel::INFO>("Init", "Hello, World!");

    SDK::Init();

    Logger::Write<LogLevel::INFO>("Init", SDK::GetWFDirectory());

    Mod::Shutdown();
}

void Mod::Shutdown() {
    FreeConsole();
    Sleep(1000);
    FreeLibraryAndExitThread(Mod::ModuleHandle, 1);
}