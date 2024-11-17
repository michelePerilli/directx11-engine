#include "../InputLayout.h"
#include "../../../exception/COMException.h"
namespace Bind {
    InputLayout::InputLayout(const Graphics &gfx,
                             const std::vector<D3D11_INPUT_ELEMENT_DESC> &layout,
                             ID3DBlob *pVertexShaderBytecode) {
        // INFOMAN( gfx );

        OFT_COM_EXCEPTION(GetDevice( gfx )->CreateInputLayout(
            layout.data(),static_cast<UINT>(layout.size()),
            pVertexShaderBytecode->GetBufferPointer(),
            pVertexShaderBytecode->GetBufferSize(),
            &pInputLayout
        ));
    }

    void InputLayout::Bind(Graphics &gfx) noexcept {
        GetContext(gfx)->IASetInputLayout(pInputLayout.Get());
    }
}