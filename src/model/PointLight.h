#pragma once

#include "SolidSphere.h"
#include "../component/pipeline/BindableBase.h"

class ENGINE_DLL PointLight
{
public:
	PointLight( Graphics& gfx,float radius = 0.1f );
	void SpawnControlWindow() noexcept;
	void Reset() noexcept;
	void Draw( Graphics& gfx ) const noexcept;
	void Bind( Graphics& gfx,DirectX::FXMMATRIX view ) const noexcept;

	struct PointLightCBuf
	{
		alignas(16) DirectX::XMFLOAT3 pos;
		alignas(16) DirectX::XMFLOAT3 ambient;
		alignas(16) DirectX::XMFLOAT3 diffuseColor;
		float diffuseIntensity;
		float attConst;
		float attLin;
		float attQuad;
	};

	PointLightCBuf cbData;
	mutable SolidSphere mesh;
	mutable PixelConstantBuffer<PointLightCBuf> cbuf;
};