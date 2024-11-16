#pragma once
#include <vector>

#include "Bindable.h"
#include "../../exception/COMException.h"

class VertexBuffer : public Bindable {
public:
    template<class V>
    VertexBuffer(Graphics &gfx, const std::vector<V> &vertices)
        : stride(sizeof(V)) {
        // INFOMAN( gfx );

        D3D11_BUFFER_DESC bd = {};
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.CPUAccessFlags = 0u;
        bd.MiscFlags = 0u;
        bd.ByteWidth = static_cast<UINT>(sizeof(V) * vertices.size());
        bd.StructureByteStride = sizeof(V);
        D3D11_SUBRESOURCE_DATA sd = {};
        sd.pSysMem = vertices.data();
        OFT_COM_EXCEPTION(GetDevice( gfx )->CreateBuffer( &bd,&sd,&pVertexBuffer ));
    }

    void Bind(Graphics &gfx) noexcept override;

protected:
    UINT stride;
    Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
};
