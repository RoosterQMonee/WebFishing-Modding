#pragma once

#include <unordered_map>
#include <iostream>
#include <string>


// may be a bit overkill for this


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
