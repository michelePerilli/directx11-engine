#pragma once
#include "../component/pipeline/Drawable.h"

class ENGINE_DLL SolidSphere : public Drawable {
public:
    SolidSphere(Graphics &gfx, float radius);

    void SetPos(DirectX::XMFLOAT3 pos) noexcept;

    [[nodiscard]] DirectX::XMMATRIX GetTransformXM() const noexcept override;

private:
    DirectX::XMFLOAT3 pos = {1.0f, 1.0f, 1.0f};
};
