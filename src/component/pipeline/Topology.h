#pragma once
#include "Bindable.h"
#include "../../core/CoreMacro.h"
namespace Bind {
    class ENGINE_DLL Topology : public Bindable {
    public:
        Topology(Graphics &gfx, D3D11_PRIMITIVE_TOPOLOGY type);

        void Bind(Graphics &gfx) noexcept override;

    protected:
        D3D11_PRIMITIVE_TOPOLOGY type;
    };
}