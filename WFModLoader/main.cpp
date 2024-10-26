#include <iostream>
#include <windows.h>
#include <filesystem>
#include <format>

#include "Utils/Memory.h"
#include "Utils/Logger.h"

#include "build.h"


int main() {
	Version v = BuildInfo::CurrentVersion;
	Logger::Write<LogLevel::INFO>("Init", "WebFishing Mod Loader ", std::format("v{}.{}{}", v.Major, v.Minor, v.Suffix));

	Memory::Init("WEBFISHING", true);
	Memory::LLInject((std::filesystem::current_path().string() + "\\WFCore.dll").c_str());

	system("pause");
	return 0;
}