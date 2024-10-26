#include "Files.h"
#include <Utils/Logger.h>

#include <filesystem>


void Files::CheckFileStructure(std::string path) {
	if (!std::filesystem::exists(path + "\\wfmodder")) {
		Logger::Write<LogLevel::WARN>("Files", "WFModder folder doesn't exist, creating folder.");
		std::filesystem::create_directories(path + "\\wfmodder");
	}

	if (!std::filesystem::exists(path + "\\wfmodder\\mods")) {
		Logger::Write<LogLevel::WARN>("Files", "Mods folder doesn't exist, creating folder.");
		std::filesystem::create_directories(path + "\\wfmodder\\mods");
	}

	if (!std::filesystem::exists(path + "\\wfmodder\\config")) {
		Logger::Write<LogLevel::WARN>("Files", "Config folder doesn't exist, creating folder.");
		std::filesystem::create_directories(path + "\\wfmodder\\config");
	}
}