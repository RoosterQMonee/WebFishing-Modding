#pragma once

#include <Windows.h>
#include <unordered_map>
#include <iostream>
#include <string>


enum class LogLevel {
    DEBUG,
    INFO,
    WARN,
    ERR
};


class Logger {
private:
    static std::unordered_map<LogLevel, std::string> levelPrefixes;

public:
    static void CreateConsole(LPCSTR title) {
        AllocConsole();
        AttachConsole(GetCurrentProcessId());
        freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);

        SetConsoleTitleA(title);

        Logger::Write<LogLevel::INFO>("Init", "Created Console");
    }

    template<LogLevel ll, class T, class ... U >
    static void Write(std::string sender, const T& first, const U&... rest)
    {
        std::cout << levelPrefixes[ll] << "[" << sender << "] " << first;
        ((std::cout << rest), ...);
        std::cout << std::endl;
    }

    template<LogLevel ll, class T, class ... U >
    static void WriteCR(std::string sender, const T& first, const U&... rest)
    {
        std::cout << levelPrefixes[ll] << "[" << sender << "] " << first;
        ((std::cout << rest), ...);
        std::cout << "\r";
    }
};
