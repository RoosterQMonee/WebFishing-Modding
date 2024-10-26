#include <TemplateMod/Mod.h>


BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, [[maybe_unused]] LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule);
        Mod::ModuleHandle = hModule;
        CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(Mod::Init), nullptr, 0, nullptr);
    }

    return TRUE;
}

