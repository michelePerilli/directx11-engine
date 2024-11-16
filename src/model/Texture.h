#pragma once

#include "../component/pipeline/Bindable.h"

class ENGINE_DLL Texture : public Bindable
{
public:
	Texture( Graphics& gfx,const class Surface& s );
	void Bind( Graphics& gfx ) noexcept override;
protected:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView;
};
