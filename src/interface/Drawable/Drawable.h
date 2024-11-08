#pragma once
#include "../Bindable/BindableBase.h"
#include <DirectXMath.h>


class ENGINE_DLL Drawable {
    template<class T>
    friend class DrawableBase;

public:
    Drawable() = default;

    Drawable(const Drawable &) = delete;

    [[nodiscard]] virtual DirectX::XMMATRIX GetTransformXM() const noexcept = 0;

    void Draw(Graphics &gfx) const noexcept;

    virtual void Update(float dt) noexcept = 0;

    virtual ~Drawable() = default;

protected:
    void AddBind(std::unique_ptr<Bindable> bind) noexcept;

    void AddIndexBuffer(std::unique_ptr<IndexBuffer> ibuf) noexcept;

private:
    [[nodiscard]] virtual const std::vector<std::unique_ptr<Bindable> > &GetStaticBinds() const noexcept = 0;


    const IndexBuffer *pIndexBuffer = nullptr;
    std::vector<std::unique_ptr<Bindable> > binds;
};
