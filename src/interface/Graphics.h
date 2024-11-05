#pragma once

#include "../include/WinSDK.h"
#include <d3d11.h>

class Graphics {
    ID3D11Device *pDevice = nullptr;
    IDXGISwapChain *pSwapChain = nullptr;
    ID3D11DeviceContext *pDeviceContext = nullptr;
    ID3D11RenderTargetView *pRenderTargetView = nullptr;

public:
    explicit Graphics(HWND hWnd) {
        DXGI_SWAP_CHAIN_DESC sd = {};
        sd.BufferDesc.Width = 0;
        sd.BufferDesc.Height = 0;
        sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.BufferDesc.RefreshRate.Numerator = 0;
        sd.BufferDesc.RefreshRate.Denominator = 0;
        sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.BufferCount = 1;
        sd.OutputWindow = hWnd;
        sd.Windowed = TRUE;
        sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        sd.Flags = 0;
        D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &sd,
                                      &pSwapChain, &pDevice, nullptr, &pDeviceContext);
        ID3D11Resource *pBackBuffer = nullptr;
        pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void **>(&pBackBuffer));
        pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &pRenderTargetView);
        pBackBuffer->Release();
    }

    Graphics(const Graphics &) = delete;

    Graphics &operator=(const Graphics &) = delete;

    ~Graphics() {
        if (pSwapChain != nullptr) {
            pSwapChain->Release();
        }
        if (pDeviceContext != nullptr) {
            pDeviceContext->Release();
        }
        if (pDevice != nullptr) {
            pDevice->Release();
        }
        if (pRenderTargetView != nullptr) {
            pRenderTargetView->Release();
        }
    }

    void EndFrame() const {
        pSwapChain->Present(1, 0);
    }

    void ClearBuffer(const float red, const float green, const float blue) const noexcept {
        const float color[] = {red, green, blue, 1.0f};
        pDeviceContext->ClearRenderTargetView(pRenderTargetView, color);
    }
};
