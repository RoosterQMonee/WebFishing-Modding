#pragma once

#include <windows.h>
#include <filesystem>
#include <TlHelp32.h>
#include <shlobj.h>
#include <iostream>
#include <Psapi.h>
#include <sstream>
#include <format>
#include <cstdio>
#include <string>


LONG WINAPI _UnhandledExceptionsHandler(EXCEPTION_POINTERS* ExceptionInfo);
static int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData);
void ManualSearch(std::string saved_path);