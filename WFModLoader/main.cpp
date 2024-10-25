#include <iostream>
#include <windows.h>
#include <format>

#include "Utils/Memory.h"
#include "Utils/Logger.h"

#include "build.h"


int main() {
	Version v = BuildInfo::CurrentVersion;
	Logger::Write<LogLevel::INFO>("Init", "WebFishing Mod Loader ", std::format("v{}.{}{}", v.Major, v.Minor, v.Suffix));
}