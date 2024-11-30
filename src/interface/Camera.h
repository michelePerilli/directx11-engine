#pragma once
#include "Graphics.h"

class ENGINE_DLL Camera
{
public:
	Camera();
	DirectX::XMMATRIX GetMatrix() const noexcept;
	void SpawnControlWindow() noexcept;
	void Reset() noexcept;
	void Rotate( float dx, float dy);
	void Translate( DirectX::XMFLOAT3 translation);


	DirectX::XMFLOAT3 pos;
	float pitch;
	float yaw;
	static constexpr float travelSpeed = 12.0f;
	static constexpr float rotationSpeed = 0.004f;
};