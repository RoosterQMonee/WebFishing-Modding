#pragma once

#include <Utils/Logger.h>
#include <WFCore/Core.h>

#ifdef __cplusplus
extern "C" {
#endif

	__declspec(dllexport) Version GetWFCoreVersion() {
		return WFCore::BuildVersion;
	}

	__declspec(dllexport) void WriteLine(const char* message) {
		Logger::WriteF(message);
	}

	__declspec(dllexport) std::string GetWebFishingDir() {
		return WFCore::WFDirectory;
	}

#ifdef __cplusplus
}
#endif