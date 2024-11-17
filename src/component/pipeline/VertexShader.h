#pragma once
#include <string>

#include "Bindable.h"
namespace Bind {
	class ENGINE_DLL VertexShader : public Bindable
	{
	public:
		VertexShader( Graphics& gfx,const std::wstring& path );
		void Bind( Graphics& gfx ) noexcept override;
		[[nodiscard]] ID3DBlob* GetBytecode() const noexcept;
	protected:
		Microsoft::WRL::ComPtr<ID3DBlob> pBytecodeBlob;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
	};
}