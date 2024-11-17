#include "../Bindable.h"
namespace Bind {
    ID3D11DeviceContext *Bindable::GetContext(const Graphics &gfx) noexcept {
        return gfx.pDeviceContext.Get();
    }

    ID3D11Device *Bindable::GetDevice(const Graphics &gfx) noexcept {
        return gfx.pDevice.Get();
    }
}