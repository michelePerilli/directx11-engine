#pragma once
#include <vector>

#include "Bindable.h"
namespace Bind {
	class ENGINE_DLL InputLayout : public Bindable
	{
	public:
		InputLayout(const Graphics& gfx,
					const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout,
					ID3DBlob* pVertexShaderBytecode );
		void Bind( Graphics& gfx ) noexcept override;
	protected:
		Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
	};
}