#pragma once

#include <Windows.h>
#include <cstdint>
#include <SDK/structs.h>


class WFCore {
public:
    static inline bool Initialized;
    static inline Version BuildVersion;
    static inline std::string WFDirectory;

    static inline HWND Window;
    static inline HMODULE ModuleHandle;

    static void Init();
    static void Shutdown();
};