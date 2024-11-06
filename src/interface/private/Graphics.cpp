#include "../Graphics.h"
#include "../../exception/COMException.h"
#include <d3dcompiler.h>
#include "../../common/Logger.h"

inline Graphics::Graphics(HWND hWnd) {
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

    Microsoft::WRL::ComPtr<ID3D11Resource> pBackBuffer = nullptr;

    OFT_COM_EXCEPTION(pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer));
    OFT_COM_EXCEPTION(pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &pRenderTargetView));
}

void Graphics::EndFrame() const {
    OFT_COM_EXCEPTION(pSwapChain->Present(1, 0));
}

void Graphics::ClearBuffer(const float red, const float green, const float blue) const noexcept {
    const float color[] = {red, green, blue, 1.0f};
    pDeviceContext->ClearRenderTargetView(pRenderTargetView.Get(), color);
}

void Graphics::DrawTestTriangle() const {
    struct Vertex {
        float x, y;
    };

    constexpr Vertex vertices[] = {
        {0.0f, 0.5f},
        {0.5f, -0.5f},
        {-0.5f, -0.5f}
    };

    Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
    D3D11_BUFFER_DESC bd = {};
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.CPUAccessFlags = 0;
    bd.MiscFlags = 0;
    bd.ByteWidth = sizeof(vertices);
    bd.StructureByteStride = sizeof(Vertex);


    D3D11_SUBRESOURCE_DATA sd = {};
    sd.pSysMem = vertices;
    OFT_COM_EXCEPTION(pDevice->CreateBuffer(&bd, &sd, &pVertexBuffer));

    constexpr UINT stride = sizeof(Vertex);
    constexpr UINT offset = 0;
    pDeviceContext->IASetVertexBuffers(0, 1, pVertexBuffer.GetAddressOf(), &stride, &offset);

    Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
    Microsoft::WRL::ComPtr<ID3DBlob> pBlob;

    // Pixel Shader
    Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
    OFT_COM_EXCEPTION(D3DReadFileToBlob(L"../../GameEngine/shader/pixel-shader.cso", &pBlob));
    OFT_COM_EXCEPTION(pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader));
    pDeviceContext->PSSetShader(pPixelShader.Get(), nullptr, 0);

    // Vertex Shader
    Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
    OFT_COM_EXCEPTION(D3DReadFileToBlob(L"../../GameEngine/shader/vertex-shader.cso", &pBlob));
    OFT_COM_EXCEPTION(pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader));
    pDeviceContext->VSSetShader(pVertexShader.Get(), nullptr, 0);

    constexpr D3D11_INPUT_ELEMENT_DESC layout[] = {
        {"Position", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };

    OFT_COM_EXCEPTION(pDevice->CreateInputLayout(layout, std::size(layout), pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &pInputLayout));
    pDeviceContext->IASetInputLayout(pInputLayout.Get());

    pDeviceContext->OMSetRenderTargets(1, pRenderTargetView.GetAddressOf(), nullptr);

    pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    D3D11_VIEWPORT viewport = {};
    viewport.Height = 800;
    viewport.Width = 800;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    pDeviceContext->RSSetViewports(1, &viewport);
    pDeviceContext->Draw(std::size(vertices), 0);
}
