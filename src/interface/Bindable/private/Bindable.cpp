#include "../Bindable.h"

#include "../../Graphics.h"

ID3D11DeviceContext *Bindable::GetContext(const Graphics &gfx) noexcept {
    return gfx.pDeviceContext.Get();
}

ID3D11Device *Bindable::GetDevice(const Graphics &gfx) noexcept {
    return gfx.pDevice.Get();
}

//DxgiInfoManager& Bindable::GetInfoManager( Graphics& gfx ) noexcept(IS_DEBUG)
//{
//#ifndef NDEBUG
//	return gfx.infoManager;
//#else
//	throw std::logic_error( "YouFuckedUp! (tried to access gfx.infoManager in Release config)" );
//#endif
//}
