#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl.h>
#include "../core/CoreMacro.h"
#define RESOLUTION_X 1920.0f
#define RESOLUTION_Y 1080.0f
#define RESOLUTION_RATIO (RESOLUTION_Y / RESOLUTION_X)
#define RENDERING_DISANCE 100.0f
#define RENDERING_FOV 0.5f

class ENGINE_DLL Graphics {
    friend class Bindable;
    DirectX::XMMATRIX projection{};
    DirectX::XMMATRIX camera{};
    bool imguiEnabled = true;
    Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
    Microsoft::WRL::ComPtr<IDXGISwapChain> pSwapChain;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pRenderTargetView;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDSV;

public:
    explicit Graphics(HWND hWnd);

    Graphics(const Graphics &) = delete;

    Graphics &operator=(const Graphics &) = delete;

    ~Graphics();

    void EndFrame() const;

    void BeginFrame(float red, float green, float blue) noexcept;

    // void ClearBuffer(float red, float green, float blue) const noexcept;

    void DrawIndexed(UINT count) const noexcept;

    void SetProjection(DirectX::FXMMATRIX proj) noexcept;

    [[nodiscard]] DirectX::XMMATRIX GetProjection() const noexcept;

    void SetCamera(DirectX::FXMMATRIX camera) noexcept;
    [[nodiscard]] DirectX::XMMATRIX GetCamera() const noexcept;

    void EnableImgui() noexcept;

    void DisableImgui() noexcept;

    bool IsImguiEnabled() const noexcept;
};
