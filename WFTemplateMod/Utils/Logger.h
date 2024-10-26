#pragma once

#include <TemplateMod/Mod.h>

#include <Windows.h>
#include <unordered_map>
#include <iostream>
#include <sstream>
#include <string>


enum class LogLevel {
    DEBUG,
    INFO,
    WARN,
    ERR
};


class Logger {
private:
    using WriteLineFn = void(*)(const char*);

    static std::unordered_map<LogLevel, std::string> levelPrefixes;
    static inline WriteLineFn g_exportedWriteLine = nullptr;
    static inline std::string modName = "";

    static void GetModName() {
        char dllPath[_MAX_PATH];
        GetModuleFileNameA(Mod::ModuleHandle, dllPath, _MAX_PATH);
        modName = std::string(dllPath);
        modName = modName.substr(modName.find_last_of("/\\") + 1);
    }

public:
    static void CreateConsole() {
        HMODULE module = GetModuleHandleW(L"WFCore.dll");
        g_exportedWriteLine = reinterpret_cast<WriteLineFn>(GetProcAddress(module, "WriteLine"));

        if (g_exportedWriteLine == 0)
            MessageBoxA(NULL, "Failed getting WriteLine from WFCore", "Debug", NULL);

        GetModName();
    }

    template<LogLevel ll, class T, class ... U >
    static void Write(std::string sender, const T& first, const U&... rest)
    {
        std::ostringstream oss;
        oss << levelPrefixes[ll] << "[" << sender << "]" << "(" << modName << ") ";
        oss << first;
        ((oss << rest), ...);
        oss << "\n";
        WriteF(oss.str().c_str());
    }

    static void WriteF(const char* text) {
        g_exportedWriteLine(text);
    }
};
