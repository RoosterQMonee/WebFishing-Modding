#pragma once

#include <Windows.h>


class Mod {
public:
    static inline HMODULE ModuleHandle;

    static void Init();
    static void Shutdown();
};