#pragma once
#include "Graphics.h"

class ENGINE_DLL Camera
{
public:
	DirectX::XMMATRIX GetMatrix() const noexcept;
	void SpawnControlWindow() noexcept;
	void Reset() noexcept;
	void LookHorizontally(float delta) noexcept;
	void LookVertically(float delta) noexcept;


	float x = 20.0f;
	float y = 20.0f;
	float r = 20.0f;
	float theta = 0.0f;
	float phi = 0.0f;
	float pitch = 0.0f;
	float yaw = 0.0f;
	float roll = 0.0f;
};