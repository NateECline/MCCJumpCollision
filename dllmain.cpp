#include "pch.h"
#include <iostream>
#include <thread>
#include "hooks.h" // Include the hooks header file

// Global module handle
HMODULE g_hModule;

void Initialize() {
    // Allocate console for debugging
    AllocConsole();
    FILE* stream;
    freopen_s(&stream, "CONOUT$", "w", stdout);
    freopen_s(&stream, "CONOUT$", "w", stderr);

    std::cout << "Starting trainer initialization..." << std::endl;

    // Call the trainer initialization
    InitializeTrainer();

    std::cout << "Trainer initialized successfully." << std::endl;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        g_hModule = hModule;
        DisableThreadLibraryCalls(hModule); // Disable thread notifications for efficiency
        std::thread(Initialize).detach();  // Run initialization in a separate thread
        break;
    case DLL_PROCESS_DETACH:
        // Clean up resources if necessary
        FreeConsole(); // Release the debugging console
        break;
    }
    return TRUE;
}