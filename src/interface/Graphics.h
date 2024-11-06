#pragma once

#include <d3d11.h>
#include <wrl.h>
#include "../core/Core.h"

class ENGINE_DLL Graphics {

    Microsoft::WRL::ComPtr<ID3D11Device> pDevice = nullptr;
    Microsoft::WRL::ComPtr<IDXGISwapChain> pSwapChain = nullptr;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext = nullptr;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pRenderTargetView = nullptr;

public:
    explicit Graphics(HWND hWnd);

    Graphics(const Graphics &) = delete;

    Graphics &operator=(const Graphics &) = delete;

    ~Graphics() = default;

    void EndFrame() const;

    void ClearBuffer(float red, float green, float blue) const noexcept;

    void DrawTestTriangle() const;
};
