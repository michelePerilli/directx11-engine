#pragma once

#pragma once
#include "DrawableBase.h"
#include <random>

class ENGINE_DLL SkinnedBox : public DrawableBase<SkinnedBox> {
public:
    SkinnedBox(Graphics &gfx, std::mt19937 &rng,
               std::uniform_real_distribution<float> &adist,
               std::uniform_real_distribution<float> &ddist,
               std::uniform_real_distribution<float> &odist,
               std::uniform_real_distribution<float> &rdist);

    explicit SkinnedBox(Graphics &gfx);

    void Update(float dt) noexcept override;

    [[nodiscard]] DirectX::XMMATRIX GetTransformXM() const noexcept override;


    void init(Graphics &gfx);

private:
	// positional
	float r;
	float roll = 0.0f;
	float pitch = 0.0f;
	float yaw = 0.0f;
	float theta;
	float phi;
	float chi;
	// speed (delta/s)
	float droll;
	float dpitch;
	float dyaw;
	float dtheta;
	float dphi;
	float dchi;
};