#pragma once
#include <string>

#include "../../interface/Graphics.h"
#include "../../core/CoreMacro.h"

namespace Bind {
    class ENGINE_DLL Bindable {
    public:
        virtual void Bind(Graphics &gfx) noexcept = 0;
        virtual std::string GetUID() const noexcept
        {
            assert( false );
            return "";
        }
        virtual ~Bindable() = default;

    protected:
        static ID3D11DeviceContext * GetContext(const Graphics &gfx) noexcept;
        static ID3D11Device * GetDevice(const Graphics &gfx) noexcept;
    };
}