#include "../VertexShader.h"

#include <codecvt>

#include "../../../exception/COMException.h"
#include <d3dcompiler.h>
#include <iostream>
#include <locale>

VertexShader::VertexShader(Graphics &gfx, const std::wstring &path) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;

    OFT_COM_EXCEPTION(D3DReadFileToBlob( path.c_str(),&pBytecodeBlob ));
    OFT_COM_EXCEPTION(GetDevice( gfx )->CreateVertexShader(
        pBytecodeBlob->GetBufferPointer(),
        pBytecodeBlob->GetBufferSize(),
        nullptr,
        &pVertexShader
    ));
}

void VertexShader::Bind(Graphics &gfx) noexcept {
    GetContext(gfx)->VSSetShader(pVertexShader.Get(), nullptr, 0u);
}

ID3DBlob *VertexShader::GetBytecode() const noexcept {
    return pBytecodeBlob.Get();
}
