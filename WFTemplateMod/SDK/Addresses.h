#pragma once

#include <Windows.h>
#include <SDK/Core.h>
#include <string>


#define FindExport(tmpl, name) reinterpret_cast<tmpl>(GetProcAddress(Addresses::WFCore, name));


namespace Templates {
	using GetWFCoreVersionTemplate = Version(*)(void);
	using GetWFDir = std::string(*)(void);
};

namespace Addresses {
	inline HMODULE WFCore = nullptr;
	inline Templates::GetWFCoreVersionTemplate GetWFCoreVersion = nullptr;
	inline Templates::GetWFDir GetWFDir = nullptr;
};
