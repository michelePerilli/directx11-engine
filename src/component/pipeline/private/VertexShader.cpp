#include "../VertexShader.h"
#include "../BindableCodex.h"

#include <codecvt>

#include "../../../exception/COMException.h"
#include <d3dcompiler.h>
#include <iostream>
#include <locale>
namespace Bind {
    VertexShader::VertexShader(Graphics &gfx, const std::string &path) {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;

        OFT_COM_EXCEPTION(D3DReadFileToBlob( std::wstring{path.begin(),path.end()}.c_str(),&pBytecodeBlob ) );
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

    std::shared_ptr<Bindable> VertexShader::Resolve(Graphics &gfx, const std::string &path) {
        return Codex::Resolve<VertexShader>( gfx,path );
    }

    std::string VertexShader::GenerateUID(const std::string &path) {
        using namespace std::string_literals;
        return typeid(VertexShader).name() + "#"s + path;
    }

    std::string VertexShader::GetUID() const noexcept {
        return GenerateUID( path );
    }
}
