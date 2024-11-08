#pragma once
#include "Bindable.h"
#include "../Drawable/Drawable.h"

class Drawable;

class ENGINE_DLL TransformCbuf : public Bindable {
public:
    TransformCbuf(Graphics &gfx, const Drawable &parent);

    void Bind(Graphics &gfx) noexcept override;

private:
    static std::unique_ptr<VertexConstantBuffer<DirectX::XMMATRIX> > pVcbuf;
    const Drawable &parent;
};
