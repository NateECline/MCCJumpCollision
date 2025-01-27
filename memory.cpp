#include "memory.h"
#include <TlHelp32.h>

void* AllocateMemory(size_t size) {
    void* address = VirtualAlloc(nullptr, size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (address == nullptr) {
        std::cerr << "Failed to allocate memory of size: " << size << std::endl;
    }
    else {
        std::cout << "Allocated memory at: " << address << " of size: " << size << std::endl;
    }
    return address;
}

void FreeMemory(void* address) {
    if (!VirtualFree(address, 0, MEM_RELEASE)) {
        std::cerr << "Failed to free memory at address: " << address << ", error: " << GetLastError() << std::endl;
    }
    else {
        std::cout << "Freed memory at address: " << address << std::endl;
    }
}

bool WriteMemoryArray(uintptr_t address, const char* data, size_t size) {
    DWORD oldProtect;
    if (!VirtualProtect(reinterpret_cast<LPVOID>(address), size, PAGE_EXECUTE_READWRITE, &oldProtect)) {
        std::cerr << "Failed to change memory protection, error: " << GetLastError() << std::endl;
        return false;
    }

    memcpy(reinterpret_cast<void*>(address), data, size);

    if (!VirtualProtect(reinterpret_cast<LPVOID>(address), size, oldProtect, &oldProtect)) {
        std::cerr << "Failed to restore memory protection, error: " << GetLastError() << std::endl;
        return false;
    }

    return true;
}

