#pragma once

#include <iostream>
#include <windows.h>


class Memory {
public:
	static inline HWND gameHandle = nullptr;
	static inline DWORD processID = 0;

	static void Init(LPCSTR windowTitle);
	static bool LLInject(const char* filepath);
};