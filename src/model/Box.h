#pragma once
#include <random>

#include "TestObject.h"

class ENGINE_DLL Box : public TestObject<Box> {
public:
    Box(Graphics &gfx, std::mt19937 &rng,
        std::uniform_real_distribution<float> &adist,
        std::uniform_real_distribution<float> &ddist,
        std::uniform_real_distribution<float> &odist,
        std::uniform_real_distribution<float> &rdist,
        std::uniform_real_distribution<float> &bdist,
        DirectX::XMFLOAT3 material );

    DirectX::XMMATRIX GetTransformXM() const noexcept override;
    bool SpawnControlWindow( int id,Graphics& gfx ) noexcept;
private:
    void SyncMaterial( Graphics& gfx ) noexcept;
    struct PSMaterialConstant
    {
        DirectX::XMFLOAT3 color;
        float specularIntensity = 0.6f;
        float specularPower = 30.0f;
        float padding[3];
    } materialConstants;
    using MaterialCbuf = PixelConstantBuffer<PSMaterialConstant>;
    // model transform
    DirectX::XMFLOAT3X3 mt{};
};
