#ifndef MEMORY_H
#define MEMORY_H

#include <cstdint>
#include <iostream>
#include <windows.h>

// Memory management functions
void* AllocateMemory(size_t size);
void FreeMemory(void* address);
bool WriteMemoryArray(uintptr_t address, const char* data, size_t size);

// Template functions for reading and writing memory
template <typename T>
T ReadMemory(uintptr_t address) {
    T value{};
    if (ReadProcessMemory(GetCurrentProcess(), reinterpret_cast<LPCVOID>(address), &value, sizeof(T), nullptr)) {
        std::cout << "Read memory from address: " << std::hex << address << " value: " << value << std::endl;
    }
    else {
        std::cerr << "Failed to read memory from address: " << std::hex << address << std::endl;
    }
    return value;
}

template <typename T>
void WriteMemory(uintptr_t address, T value) {
    if (WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<LPVOID>(address), &value, sizeof(T), nullptr)) {
        std::cout << "Wrote memory to address: " << std::hex << address << " value: " << value << std::endl;
    }
    else {
        std::cerr << "Failed to write memory to address: " << std::hex << address << std::endl;
    }
}



#endif // MEMORY_H
