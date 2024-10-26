#pragma once

#include <Utils/Logger.h>

#ifdef __cplusplus
extern "C" {
#endif

	__declspec(dllexport) void WriteLine(const char* message) {
		Logger::WriteF(message);
	}

#ifdef __cplusplus
}
#endif