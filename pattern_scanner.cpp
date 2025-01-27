#include "pattern_scanner.h"
#include <Psapi.h>
#include <iostream>
#include <vector>

uintptr_t PatternScan(const char* moduleName, const char* pattern) {
    // Get module handle
    HMODULE hModule = GetModuleHandleA(moduleName);
    if (!hModule) {
        std::cerr << "Failed to get module handle for: " << moduleName << std::endl;
        return 0;
    }

    // Get module information
    MODULEINFO moduleInfo;
    if (!GetModuleInformation(GetCurrentProcess(), hModule, &moduleInfo, sizeof(moduleInfo))) {
        std::cerr << "Failed to get module information for: " << moduleName << std::endl;
        return 0;
    }

    uintptr_t startAddress = (uintptr_t)moduleInfo.lpBaseOfDll;
    uintptr_t endAddress = startAddress + moduleInfo.SizeOfImage;

    // Parse pattern
    std::vector<int> patternBytes;
    const char* current = pattern;
    while (*current) {
        if (*current == '?') {
            patternBytes.push_back(-1); // Wildcard
            current++;
            if (*current == '?') current++; // Skip double ??
        }
        else {
            patternBytes.push_back(strtoul(current, (char**)&current, 16));
        }
    }

    // Scan memory
    for (uintptr_t currentAddress = startAddress; currentAddress < endAddress - patternBytes.size(); ++currentAddress) {
        bool found = true;
        for (size_t i = 0; i < patternBytes.size(); i++) {
            if (patternBytes[i] != -1 && *(uint8_t*)(currentAddress + i) != patternBytes[i]) {
                found = false;
                break;
            }
        }
        if (found) {
            return currentAddress;
        }
    }

    std::cerr << "Pattern not found: " << pattern << std::endl;
    return 0;
}
