#include "../PixelShader.h"
#include <d3dcompiler.h>

#include "../../../exception/COMException.h"

PixelShader::PixelShader(Graphics &gfx, const std::wstring &path) {
    // INFOMAN( gfx );

    Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
    OFT_COM_EXCEPTION(D3DReadFileToBlob( path.c_str(),&pBlob ));
    OFT_COM_EXCEPTION(
        GetDevice( gfx )->CreatePixelShader( pBlob->GetBufferPointer(),pBlob->GetBufferSize(),nullptr,&pPixelShader ));
}

void PixelShader::Bind(Graphics &gfx) noexcept {
    GetContext(gfx)->PSSetShader(pPixelShader.Get(), nullptr, 0u);
}
