#pragma once
#include "../pipeline/BindableBase.h"
#include <DirectXMath.h>
#include "../../core/CoreMacro.h"

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
    template<class T>
    T *QueryBindable() noexcept {
        for (auto &pb: binds) {
            if (auto pt = dynamic_cast<T *>(pb.get())) {
                return pt;
            }
        }
        return nullptr;
    }

    void AddBind(std::unique_ptr<Bindable> bind) noexcept;

    void AddIndexBuffer(std::unique_ptr<IndexBuffer> ibuf) noexcept;

private:
    [[nodiscard]] virtual const std::vector<std::unique_ptr<Bindable> > &GetStaticBinds() const noexcept = 0;


    const IndexBuffer *pIndexBuffer = nullptr;
    std::vector<std::unique_ptr<Bindable> > binds;
};
