#pragma once
#include <memory>

#include "Bindable.h"
#include "Drawable.h"
#include "../../core/CoreMacro.h"

class Drawable;
namespace Bind {
    class ENGINE_DLL TransformCbuf : public Bindable {
    private:
        struct Transforms {
            DirectX::XMMATRIX modelViewProj;
            DirectX::XMMATRIX model;
        };

    public:
        TransformCbuf(Graphics &gfx, const Drawable &parent, UINT slot = 0u);

        void Bind(Graphics &gfx) noexcept override;

    private:
        static std::unique_ptr<VertexConstantBuffer<Transforms> > pVcbuf;
        const Drawable &parent;
    };
}