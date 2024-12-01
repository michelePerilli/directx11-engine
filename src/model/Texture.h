#pragma once

#include "Surface.h"
#include "../component/pipeline/Bindable.h"

namespace Bind {
    class ENGINE_DLL Texture : public Bindable {
    public:
        Texture(Graphics &gfx, const Surface &s, unsigned int slot = 0);

        void Bind(Graphics &gfx) noexcept override;

    private:
        unsigned int slot;

    protected:
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView;
    };
}
