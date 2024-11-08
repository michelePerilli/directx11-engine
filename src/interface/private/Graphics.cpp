#include "../Graphics.h"
#include "../../exception/COMException.h"
#include <d3dcompiler.h>

#include <DirectXMath.h>

#include "../../common/Logger.h"

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
    descDepth.Width = 800;
    descDepth.Height = 600;
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
    vp.Width = 800;
    vp.Height = 600;
    vp.MinDepth = 0.5f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0.0f;
    vp.TopLeftY = 0.0f;
    pDeviceContext->RSSetViewports( 1u,&vp );
}

void Graphics::EndFrame() const {
    OFT_COM_EXCEPTION(pSwapChain->Present(1, 0));
}

void Graphics::ClearBuffer(const float red, const float green, const float blue) const noexcept {
    const float color[] = {red, green, blue, 1.0f};
    pDeviceContext->ClearRenderTargetView(pRenderTargetView.Get(), color);
    pDeviceContext->ClearDepthStencilView(pDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Graphics::DrawIndexed(const UINT count) const noexcept {
    pDeviceContext->DrawIndexed( count,0u,0u );
}

void Graphics::SetProjection(DirectX::FXMMATRIX proj) noexcept {
    projection = proj;
}

DirectX::XMMATRIX Graphics::GetProjection() const noexcept {
    return projection;
}
//
// void Graphics::DrawTestTriangle(const float angle, float x, float y) const {
//     struct Vertex {
//         struct {
//             float x;
//             float y;
//             float z;
//         } position;
//     };
//
//     Vertex vertices[] = {
//         {-1.0f, -1.0f, -1.0f},
//         {1.0f, -1.0f, -1.0f},
//         {-1.0f, 1.0f, -1.0f},
//         {1.0f, 1.0f, -1.0f},
//         {-1.0f, -1.0f, 1.0f},
//         {1.0f, -1.0f, 1.0f},
//         {-1.0f, 1.0f, 1.0f},
//         {1.0f, 1.0f, 1.0f},
//     };
//
//
//     Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
//     D3D11_BUFFER_DESC bd = {};
//     bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//     bd.Usage = D3D11_USAGE_DEFAULT;
//     bd.CPUAccessFlags = 0;
//     bd.MiscFlags = 0;
//     bd.ByteWidth = sizeof(vertices);
//     bd.StructureByteStride = sizeof(Vertex);
//
//
//     D3D11_SUBRESOURCE_DATA sd = {};
//     sd.pSysMem = vertices;
//     OFT_COM_EXCEPTION(pDevice->CreateBuffer(&bd, &sd, &pVertexBuffer));
//
//     constexpr UINT stride = sizeof(Vertex);
//     constexpr UINT offset = 0;
//     pDeviceContext->IASetVertexBuffers(0, 1, pVertexBuffer.GetAddressOf(), &stride, &offset);
//
//
//     const unsigned short indices[] = {
//         0, 2, 1, 2, 3, 1,
//         1, 3, 5, 3, 7, 5,
//         2, 6, 3, 3, 6, 7,
//         4, 5, 7, 4, 7, 6,
//         0, 4, 2, 2, 4, 6,
//         0, 1, 4, 1, 5, 4,
//     };
//
//     Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;
//     D3D11_BUFFER_DESC ibd = {};
//     ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
//     ibd.Usage = D3D11_USAGE_DEFAULT;
//     ibd.CPUAccessFlags = 0;
//     ibd.MiscFlags = 0;
//     ibd.ByteWidth = sizeof(indices);
//     ibd.StructureByteStride = sizeof(unsigned short);
//     D3D11_SUBRESOURCE_DATA isd = {};
//     isd.pSysMem = indices;
//     OFT_COM_EXCEPTION(pDevice->CreateBuffer(&ibd, &isd, &pIndexBuffer));
//
//     pDeviceContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
//
//     struct ConstantBuffer {
//         DirectX::XMMATRIX transform;
//     };
//
//     const ConstantBuffer cb = {
//         {
//             XMMatrixTranspose(
//                 DirectX::XMMatrixRotationZ(angle) *
//                 DirectX::XMMatrixRotationX(angle) *
//                 DirectX::XMMatrixTranslation(x, y, 4) *
//                 DirectX::XMMatrixPerspectiveLH(1, 3.f/4.f, 0.5f, 10)
//             )
//         }
//     };
//     Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer;
//     D3D11_BUFFER_DESC cbd = {};
//     cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
//     cbd.Usage = D3D11_USAGE_DYNAMIC;
//     cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
//     cbd.MiscFlags = 0;
//     cbd.ByteWidth = sizeof(cb);
//     cbd.StructureByteStride = 0;
//     D3D11_SUBRESOURCE_DATA csd = {};
//     csd.pSysMem = &cb;
//     OFT_COM_EXCEPTION(pDevice->CreateBuffer(&cbd, &csd, &pConstantBuffer));
//     pDeviceContext->VSSetConstantBuffers(0, 1, pConstantBuffer.GetAddressOf());
//
//
//     struct ConstantBufferColor {
//         struct {
//             float r;
//             float g;
//             float b;
//             float a;
//         } face_colors[6];
//     };
//
//     const ConstantBufferColor cbc = {
//         {
//             {1,0,1},
//             {1,0,0},
//             {0,1,0},
//             {0,0,1},
//             {1,1,0},
//             {0,1,1},
//
//         }
//     };
//
//
//     Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBufferColors;
//     D3D11_BUFFER_DESC cbdc = {};
//     cbdc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
//     cbdc.Usage = D3D11_USAGE_DEFAULT;
//     cbdc.CPUAccessFlags = 0;
//     cbdc.MiscFlags = 0;
//     cbdc.ByteWidth = sizeof(cbc);
//     cbdc.StructureByteStride = 0;
//     D3D11_SUBRESOURCE_DATA csdc = {};
//     csdc.pSysMem = &cbc;
//     OFT_COM_EXCEPTION(pDevice->CreateBuffer(&cbdc, &csdc, &pConstantBufferColors));
//     pDeviceContext->VSSetConstantBuffers(0, 1, pConstantBufferColors.GetAddressOf());
//
//
//     Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
//
//     // Pixel Shader
//     Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
//     OFT_COM_EXCEPTION(D3DReadFileToBlob(L"../../GameEngine/shader/ColorIndexPS.cso", &pBlob));
//     OFT_COM_EXCEPTION(
//         pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader));
//     pDeviceContext->PSSetShader(pPixelShader.Get(), nullptr, 0);
//
//     // Vertex Shader
//     Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
//     OFT_COM_EXCEPTION(D3DReadFileToBlob(L"../../GameEngine/shader/ColorIndexVS.cso", &pBlob));
//     OFT_COM_EXCEPTION(
//         pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader));
//     pDeviceContext->VSSetShader(pVertexShader.Get(), nullptr, 0);
//
//     Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
//     constexpr D3D11_INPUT_ELEMENT_DESC layout[] = {
//         {"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
//         // {"Color", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
//     };
//
//     OFT_COM_EXCEPTION(
//         pDevice->CreateInputLayout(layout, std::size(layout), pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &
//             pInputLayout));
//     pDeviceContext->IASetInputLayout(pInputLayout.Get());
//
//     pDeviceContext->OMSetRenderTargets(1, pRenderTargetView.GetAddressOf(), nullptr);
//
//     pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//
//     D3D11_VIEWPORT viewport = {};
//     viewport.Width = 400;
//     viewport.Height = 300;
//     viewport.MinDepth = 0;
//     viewport.MaxDepth = 1;
//     viewport.TopLeftX = 100;
//     viewport.TopLeftY = 100;
//     pDeviceContext->RSSetViewports(1, &viewport);
//
//     // pDeviceContext->Draw(std::size(vertices), 0);
//     pDeviceContext->DrawIndexed(std::size(indices), 0, 0);
// }
