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

    void AddBind(std::shared_ptr<Bind::Bindable> bind) noexcept;


private:
    const Bind::IndexBuffer *pIndexBuffer = nullptr;
    std::vector<std::shared_ptr<Bind::Bindable> > binds;
};
