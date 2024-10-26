#pragma once

#include <Windows.h>
#include <cstdint>
#include <SDK/structs.h>
#include <string>


class WFCore {
public:
    static inline bool Initialized = false;
    static inline Version BuildVersion = {1,0,'a'};
    static inline std::string WFDirectory = "unknown";

    static inline HWND Window;
    static inline HMODULE ModuleHandle;

    static void Init();
    static void Shutdown();
};