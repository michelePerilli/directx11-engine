#pragma once
#include <string>

#include "Bindable.h"
namespace Bind {
	class ENGINE_DLL PixelShader : public Bindable
	{
	public:
		PixelShader( Graphics& gfx,const std::wstring& path );
		void Bind( Graphics& gfx ) noexcept override;
	protected:
		Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
	};
}