#include "../Graphics.h"
#include "../../exception/COMException.h"
#include <d3dcompiler.h>

#include <DirectXMath.h>

#include "../../common/Logger.h"
#include "../../libs/imgui/imgui_impl_dx11.h"
#include "../../libs/imgui/imgui_impl_win32.h"

Graphics::Graphics(HWND hWnd) {
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

    Logger::info("Creating DXGI Swap Chain...");

    OFT_COM_EXCEPTION(
        D3D11CreateDeviceAndSwapChain(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            0,
            nullptr,
            0,
            D3D11_SDK_VERSION,
            &sd,
            &pSwapChain,
            &pDevice,
            nullptr,
            &pDeviceContext
        ));

    Microsoft::WRL::ComPtr<ID3D11Resource> pBackBuffer;

    OFT_COM_EXCEPTION(pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer));
    OFT_COM_EXCEPTION(pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &pRenderTargetView));

    D3D11_DEPTH_STENCIL_DESC dsDesc = {};
    dsDesc.DepthEnable = TRUE;
    dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilState> pDSState;
    OFT_COM_EXCEPTION(pDevice->CreateDepthStencilState(&dsDesc, &pDSState));

    pDeviceContext->OMSetDepthStencilState(pDSState.Get(), 1);

    Microsoft::WRL::ComPtr<ID3D11Texture2D> pDepthStencil;
    D3D11_TEXTURE2D_DESC descDepth = {};
    descDepth.Width = RESOLUTION_X;
    descDepth.Height = RESOLUTION_Y;
    descDepth.MipLevels = 1;
    descDepth.ArraySize = 1;
    descDepth.Format = DXGI_FORMAT_D32_FLOAT;
    descDepth.SampleDesc.Count = 1;
    descDepth.SampleDesc.Quality = 0;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    OFT_COM_EXCEPTION(pDevice->CreateTexture2D(&descDepth, nullptr, &pDepthStencil));

    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
    descDSV.Format = DXGI_FORMAT_D32_FLOAT;
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descDSV.Texture2D.MipSlice = 0;
    OFT_COM_EXCEPTION(pDevice->CreateDepthStencilView(pDepthStencil.Get(), &descDSV, &pDSV));

    pDeviceContext->OMSetRenderTargets(1, pRenderTargetView.GetAddressOf(), pDSV.Get());

    // configure viewport
    D3D11_VIEWPORT vp;
    vp.Width = RESOLUTION_X;
    vp.Height = RESOLUTION_Y;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0.0f;
    vp.TopLeftY = 0.0f;
    pDeviceContext->RSSetViewports(1u, &vp);

    ImGui_ImplDX11_Init(pDevice.Get(), pDeviceContext.Get());
}

Graphics::~Graphics() {
    ImGui_ImplDX11_Shutdown();
}

void Graphics::EndFrame() const {
    // imgui frame end
    if (imguiEnabled) {
        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }

    OFT_COM_EXCEPTION(pSwapChain->Present(1, 0));
}

void Graphics::BeginFrame(float red, float green, float blue) noexcept {
    // imgui begin frame
    if (imguiEnabled) {
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
    }

    const float color[] = {red, green, blue, 1.0f};
    pDeviceContext->ClearRenderTargetView(pRenderTargetView.Get(), color);
    pDeviceContext->ClearDepthStencilView(pDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}

// void Graphics::ClearBuffer(const float red, const float green, const float blue) const noexcept {
//     const float color[] = {red, green, blue, 1.0f};
//     pDeviceContext->ClearRenderTargetView(pRenderTargetView.Get(), color);
//     pDeviceContext->ClearDepthStencilView(pDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
// }

void Graphics::DrawIndexed(const UINT count) const noexcept {
    pDeviceContext->DrawIndexed(count, 0u, 0u);
}

void Graphics::SetProjection(DirectX::FXMMATRIX proj) noexcept {
    projection = proj;
}

DirectX::XMMATRIX Graphics::GetProjection() const noexcept {
    return projection;
}

void Graphics::SetCamera(DirectX::FXMMATRIX camera) noexcept {
    this->camera = camera;
}

DirectX::XMMATRIX Graphics::GetCamera() const noexcept {
    return camera;
}

void Graphics::EnableImgui() noexcept {
    imguiEnabled = true;
}

void Graphics::DisableImgui() noexcept {
    imguiEnabled = false;
}

bool Graphics::IsImguiEnabled() const noexcept {
    return imguiEnabled;
}
