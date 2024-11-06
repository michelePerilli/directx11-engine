#pragma once

#include "../include/WinSDK.h"
#include <d3d11.h>
#include <wrl.h>
#include <d3dcompiler.h>

class ENGINE_DLL  Graphics {
    Microsoft::WRL::ComPtr<ID3D11Device> pDevice = nullptr;
    Microsoft::WRL::ComPtr<IDXGISwapChain> pSwapChain = nullptr;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext = nullptr;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pRenderTargetView = nullptr;

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
        const HRESULT result = D3D11CreateDeviceAndSwapChain(
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
        );

        if (result != S_OK) {
            throw std::runtime_error("Failed to create swap chain");
        }
        Microsoft::WRL::ComPtr<ID3D11Resource> pBackBuffer = nullptr;
        pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer);
        pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &pRenderTargetView);
    }

    Graphics(const Graphics &) = delete;

    Graphics &operator=(const Graphics &) = delete;

    ~Graphics() = default;

    void EndFrame() const {
        if (const HRESULT hresult = pSwapChain->Present(1, 0); FAILED(hresult)) {
            if (hresult == DXGI_ERROR_DEVICE_REMOVED) {
                throw std::runtime_error("Device removed or driver failed");
            }
            throw std::runtime_error("Failed to present");
        }
    }

    void ClearBuffer(const float red, const float green, const float blue) const noexcept {
        const float color[] = {red, green, blue, 1.0f};
        pDeviceContext->ClearRenderTargetView(pRenderTargetView.Get(), color);
    }

    void DrawTestTriangle() const {
        struct Vertex {
            float x, y;
        };

        const Vertex vertices[] = {
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
        if (FAILED(pDevice->CreateBuffer(&bd, &sd, &pVertexBuffer))) {
            throw std::runtime_error("Failed to create vertex buffer");
        }

        constexpr UINT stride = sizeof(Vertex);
        constexpr UINT offset = 0;
        pDeviceContext->IASetVertexBuffers(0, 1, pVertexBuffer.GetAddressOf(), &stride, &offset);

        Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
        Microsoft::WRL::ComPtr<ID3DBlob> pBlob;

        // Pixel Shader
        Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
        if (FAILED(D3DReadFileToBlob(L"../../GameEngine/shader/pixel-shader.cso", &pBlob))) {
            throw std::runtime_error("Failed to load pixel shader blob file");
        }
        if (FAILED(pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader))) {
            throw std::runtime_error("Failed to load pixel shader");
        }
        pDeviceContext->PSSetShader(pPixelShader.Get(), nullptr, 0);
        // Vertex Shader
        Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
        if (FAILED(D3DReadFileToBlob(L"../../GameEngine/shader/vertex-shader.cso", &pBlob))) {
            throw std::runtime_error("Failed to load vertex shader blob file");
        }
        if (FAILED(pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader))) {
            throw std::runtime_error("Failed to load vertex shader");
        }
        pDeviceContext->VSSetShader(pVertexShader.Get(), nullptr, 0);

        const D3D11_INPUT_ELEMENT_DESC layout[] = {
            {"Position", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
        };

        if (FAILED(pDevice->CreateInputLayout(layout, std::size(layout), pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &pInputLayout))) {
            throw std::runtime_error("Failed to create input layout");
        }
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
};
