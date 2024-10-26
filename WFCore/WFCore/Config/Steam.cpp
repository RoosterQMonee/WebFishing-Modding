#include "Steam.h"

#include <Utils/System.h>
#include <Dependencies/vdf_parser.h>

#include <windows.h>
#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>


std::string GetSteamInstallPath() {
    HKEY hKey;
    if (RegOpenKeyExA(HKEY_CURRENT_USER, "SOFTWARE\\Valve\\Steam", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        char steamPath[MAX_PATH];
        DWORD dwType = REG_SZ;
        DWORD dwSize = sizeof(steamPath);
        if (RegQueryValueExA(hKey, "SteamPath", 0, &dwType, (LPBYTE)steamPath, &dwSize) == ERROR_SUCCESS) {
            RegCloseKey(hKey);
            return std::string(steamPath);
        }
        RegCloseKey(hKey);
    }
    return "";
}

std::string GetSteamLibraryFolder(const std::string& steamInstallPath) {
    std::string libraryfoldersPath = steamInstallPath + "\\steamapps\\libraryfolders.vdf";
    std::ifstream file(libraryfoldersPath);
    auto root = tyti::vdf::read(file);
    const std::shared_ptr<tyti::vdf::object> child = root.childs["0"];

    return child->attribs["path"];
}

std::string Steam::FindLibraryFolder() {
    static std::string steamInstallPath = GetSteamInstallPath();
    if (steamInstallPath.empty()) {
        return "bad-install";
    }

    static std::string steamLibraryFolder = GetSteamLibraryFolder(steamInstallPath);
    if (steamLibraryFolder.empty()) {
        return "bad-library";
    }
    return steamLibraryFolder;
}

std::string Steam::FindWFPath() {
    std::string libFolder = Steam::FindLibraryFolder();

    if (libFolder == "bad-install" || libFolder == "bad-library")
        return libFolder;

    return libFolder + "\\steamapps\\common\\WEBFISHING";
}