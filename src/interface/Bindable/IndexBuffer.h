#pragma once
#include "Bindable.h"

class ENGINE_DLL IndexBuffer : public Bindable {
public:
    IndexBuffer(const Graphics &gfx, const std::vector<unsigned short> &indices);

    void Bind(Graphics &gfx) noexcept override;

    [[nodiscard]] UINT GetCount() const noexcept;

protected:
    UINT count;
    Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;
};
