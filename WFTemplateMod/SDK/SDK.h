#pragma once

#include "Addresses.h"
#include "Core.h"


class SDK {
public:
	static void Init() {
		Addresses::WFCore = GetModuleHandleW(L"WFCore.dll");

		Addresses::GetWFCoreVersion = FindExport(Templates::GetWFCoreVersionTemplate, "GetWFCoreVersion");
		Addresses::GetWFDir = FindExport(Templates::GetWFDir, "GetWebFishingDir");
	}

	static Version GetWFCoreVersion() {
		return Addresses::GetWFCoreVersion();
	}

	static std::string GetWFDirectory() {
		return Addresses::GetWFDir();
	}
};