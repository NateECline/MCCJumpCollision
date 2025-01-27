#include "gui.h"
#include "imgui.h"
#include "backends/imgui_impl_dx11.h"
#include "backends/imgui_impl_win32.h"
#include <iostream>

static HWND gameWindow = nullptr;
static ID3D11RenderTargetView* rtv = nullptr;
static ID3D11Device* device = nullptr;
static ID3D11DeviceContext* context = nullptr;
static WNDPROC oWndProc = nullptr;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void InitImGui(IDXGISwapChain* pSwapChain) {
    DXGI_SWAP_CHAIN_DESC sd;
    HRESULT result = pSwapChain->GetDesc(&sd);
    gameWindow = sd.OutputWindow;

    result = pSwapChain->GetDevice(__uuidof(ID3D11Device), reinterpret_cast<void**>(&device));
    device->GetImmediateContext(&context);

    ID3D11Texture2D* backBuffer = nullptr;
    result = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
    device->CreateRenderTargetView(backBuffer, NULL, &rtv);
    backBuffer->Release();

    ImGui::CreateContext();
    ImGui_ImplWin32_Init(gameWindow);
    ImGui_ImplDX11_Init(device, context);

    oWndProc = (WNDPROC)SetWindowLongPtr(gameWindow, GWLP_WNDPROC, (LONG_PTR)WndProc);

    std::cout << "ImGui initialized with window: " << gameWindow << std::endl;
}

void RenderImGui() {
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    // Simple ImGui Window
    ImGui::Begin("Ghost Detection");
    ImGui::Text("Hello, World!"); // Display "Hello, World!" message
    ImGui::End();

    // Render ImGui
    ImGui::Render();
    context->OMSetRenderTargets(1, &rtv, nullptr);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
        return true;

    return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

void CleanupRenderTarget() {
    if (rtv) {
        rtv->Release();
        rtv = nullptr;
    }
}

void CreateRenderTarget(IDXGISwapChain* pSwapChain) {
    ID3D11Texture2D* backBuffer = nullptr;
    if (FAILED(pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer))) {
        std::cout << "Failed to get back buffer." << std::endl;
        return;
    }

    ID3D11Device* device = nullptr;
    pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&device);

    if (FAILED(device->CreateRenderTargetView(backBuffer, NULL, &rtv))) {
        backBuffer->Release();
        std::cout << "Failed to create render target view." << std::endl;
        return;
    }
    backBuffer->Release();
    device->Release();
}
