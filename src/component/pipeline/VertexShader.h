#pragma once
#include <memory>

#include "Bindable.h"

namespace Bind {
    class ENGINE_DLL VertexShader : public Bindable {
    public:
        VertexShader(Graphics &gfx, const std::string &path);

        void Bind(Graphics &gfx) noexcept override;

        [[nodiscard]] ID3DBlob *GetBytecode() const noexcept;

        static std::shared_ptr<Bindable> Resolve(Graphics &gfx, const std::string &path);

        static std::string GenerateUID(const std::string &path);

        std::string GetUID() const noexcept override;

    protected:
        std::string path;
        Microsoft::WRL::ComPtr<ID3DBlob> pBytecodeBlob;
        Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
    };
}
