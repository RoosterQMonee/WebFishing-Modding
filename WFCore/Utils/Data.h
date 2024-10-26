#pragma once

#include <string>
#include <algorithm>
#include <windows.h>


std::string WString2String(const std::wstring& wstr)
{
    std::string str(wstr.length(), 0);
    std::transform(wstr.begin(), wstr.end(), str.begin(), [](wchar_t c) {
        return (char)c;
    });
    return str;
}