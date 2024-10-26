#pragma once

#include <Windows.h>
#include <atlstr.h>
#include <atldef.h>
#include <string>

class System {
public:
    static CString GetStringRegKey(HKEY hKey, PCWSTR subKey, PCWSTR valueName);
};