#include "Mod.h"

#include <Utils/Logger.h>


void Mod::Init() {
    Logger::CreateConsole();

    Logger::Write<LogLevel::INFO>("Init", "Hello, World!");

    Mod::Shutdown();
}

void Mod::Shutdown() {
    FreeConsole();
    Sleep(1000);
    FreeLibraryAndExitThread(Mod::ModuleHandle, 1);
}