#ifndef PATTERN_SCANNER_H
#define PATTERN_SCANNER_H

#include <cstdint>
#include <Windows.h>

uintptr_t PatternScan(const char* moduleName, const char* pattern);

#endif
