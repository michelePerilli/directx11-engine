#pragma once

#include "../core/CoreMin.h"
#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl.h>

class ENGINE_DLL Graphics {
    friend class Bindable;
    DirectX::XMMATRIX projection;

    Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
    Microsoft::WRL::ComPtr<IDXGISwapChain> pSwapChain;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pRenderTargetView;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDSV;

public:
    explicit Graphics(HWND hWnd);

    Graphics(const Graphics &) = delete;

    Graphics &operator=(const Graphics &) = delete;

    ~Graphics() = default;

    void EndFrame() const;

    void ClearBuffer(float red, float green, float blue) const noexcept;

    void DrawIndexed(UINT count) const noexcept;

    void SetProjection(DirectX::FXMMATRIX proj) noexcept;

    [[nodiscard]] DirectX::XMMATRIX GetProjection() const noexcept;

    // void DrawTestTriangle(float angle, float x, float y) const;
};
