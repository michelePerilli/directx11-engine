#pragma once
#include <memory>

#include "../component/pipeline/Bindable.h"
namespace Bind {
	class ENGINE_DLL Sampler : public Bindable
	{
	public:
		Sampler( Graphics& gfx );
		void Bind( Graphics& gfx ) noexcept override;
		static std::shared_ptr<Bindable> Resolve( Graphics& gfx );
		static std::string GenerateUID();
		std::string GetUID() const noexcept override;
	protected:
		Microsoft::WRL::ComPtr<ID3D11SamplerState> pSampler;
	};
}