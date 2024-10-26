#pragma once

#include <Windows.h>
#include <unordered_map>
#include <iostream>
#include <sstream>
#include <string>
#include <atomic>
#include <mutex>


enum class LogLevel {
    DEBUG,
    INFO,
    WARN,
    ERR
};


class Logger {
private:
    static std::unordered_map<LogLevel, std::string> levelPrefixes;
    static inline std::atomic<HANDLE> g_hConsole = nullptr;
    static inline std::atomic<bool> g_consoleInitialized = false;
    static inline std::mutex m_outputMutex;

public:
    static void CreateConsole(LPCSTR title) {
        AllocConsole();
        AttachConsole(GetCurrentProcessId());
        freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
        SetConsoleTitleA(title);

        if (!g_consoleInitialized.exchange(true)) {
            g_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleMode(g_hConsole, ENABLE_PROCESSED_OUTPUT | ENABLE_WRAP_AT_EOL_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
        }

        Logger::Write<LogLevel::INFO>("Init", "Created Console");
    }

    template<LogLevel ll, class T, class ... U >
    static void Write(std::string sender, const T& first, const U&... rest)
    {
        std::ostringstream oss;
        oss << levelPrefixes[ll] << "[" << sender << "] ";
        oss << first;
        ((oss << rest), ...);
        oss << "\n";
        WriteF(oss.str().c_str());

        //std::cout << levelPrefixes[ll] << "[" << sender << "] " << first;
        //((std::cout << rest), ...);
        //std::cout << std::endl;
    }

    static void WriteF(const char* format, ...) {
        std::lock_guard<std::mutex> lock(m_outputMutex);
        va_list args;
        va_start(args, format);
        DWORD written = 0;
        
        WriteConsoleA(g_hConsole, format, strlen(format), &written, nullptr);

        va_end(args);
    }

    template<LogLevel ll, class T, class ... U >
    static void WriteCR(std::string sender, const T& first, const U&... rest)
    {
        std::cout << levelPrefixes[ll] << "[" << sender << "] " << first;
        ((std::cout << rest), ...);
        std::cout << "\r";
    }
};
