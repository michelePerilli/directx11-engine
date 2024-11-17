#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <dxgiformat.h>
#include <vector>
#include <type_traits>
#include "../../core/CoreMacro.h"
// TODO da fare refactor su nomi e struttura

namespace Vertexes {
    struct BGRAColors {
        unsigned char a;
        unsigned char r;
        unsigned char g;
        unsigned char b;
    };

    enum ElementType {
        Position2D,
        Position3D,
        Texture2D,
        Normal,
        Float3Color,
        Float4Color,
        BGRAColor,
        Count,
    };

    template<ElementType>
    struct Map;

    template<>
    struct Map<Position2D> {
        using SysType = DirectX::XMFLOAT2;
        static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32_FLOAT;
        static constexpr const char *semantic = "Position";
    };

    template<>
    struct Map<Position3D> {
        using SysType = DirectX::XMFLOAT3;
        static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
        static constexpr const char *semantic = "Position";
    };

    template<>
    struct Map<Texture2D> {
        using SysType = DirectX::XMFLOAT2;
        static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32_FLOAT;
        static constexpr const char *semantic = "Texcoord";
    };

    template<>
    struct Map<Normal> {
        using SysType = DirectX::XMFLOAT3;
        static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
        static constexpr const char *semantic = "Normal";
    };

    template<>
    struct Map<Float3Color> {
        using SysType = DirectX::XMFLOAT3;
        static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
        static constexpr const char *semantic = "Color";
    };

    template<>
    struct Map<Float4Color> {
        using SysType = DirectX::XMFLOAT4;
        static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
        static constexpr const char *semantic = "Color";
    };

    template<>
    struct Map<BGRAColor> {
        using SysType = BGRAColors;
        static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
        static constexpr const char *semantic = "Color";
    };

    class ENGINE_DLL VertexLayout {
    public:
        class ENGINE_DLL Element {
        public:
            Element(ElementType type, size_t offset) noexcept;

            size_t GetOffsetAfter() const noexcept;

            size_t GetOffset() const;

            size_t Size() const noexcept;

            static constexpr size_t SizeOf(ElementType type) noexcept;

            ElementType GetType() const noexcept;

            D3D11_INPUT_ELEMENT_DESC GetDesc() const noexcept;

        private:
            template<ElementType type>
            static constexpr D3D11_INPUT_ELEMENT_DESC GenerateDesc(size_t offset) noexcept;

        private:
            ElementType type;
            size_t offset;
        };

    public:
        template<ElementType Type>
        const Element &Resolve() const noexcept;

        const Element &ResolveByIndex(size_t i) const noexcept;

        VertexLayout &Append(ElementType type) noexcept;

        size_t Size() const noexcept;

        size_t GetElementCount() const noexcept;

        std::vector<D3D11_INPUT_ELEMENT_DESC> GetD3DLayout() const noexcept;

    private:
        std::vector<Element> elements;
    };

    template<ElementType Type>
    const VertexLayout::Element & VertexLayout::Resolve() const noexcept {
        for (auto &e: elements) {
            if (e.GetType() == Type) {
                return e;
            }
        }
        assert("Could not resolve element type" && false);
        return elements.front();
    }

    template<ElementType type>
    constexpr D3D11_INPUT_ELEMENT_DESC VertexLayout::Element::GenerateDesc(size_t offset) noexcept {
        return {
            Map<type>::semantic, 0, Map<type>::dxgiFormat, 0, (UINT) offset, D3D11_INPUT_PER_VERTEX_DATA, 0
        };
    }

    constexpr size_t VertexLayout::Element::SizeOf(ElementType type) noexcept {
        switch (type) {
            case Position2D:
                return sizeof(Map<Position2D>::SysType);
            case Position3D:
                return sizeof(Map<Position3D>::SysType);
            case Texture2D:
                return sizeof(Map<Texture2D>::SysType);
            case Normal:
                return sizeof(Map<Normal>::SysType);
            case Float3Color:
                return sizeof(Map<Float3Color>::SysType);
            case Float4Color:
                return sizeof(Map<Float4Color>::SysType);
            case BGRAColor:
                return sizeof(Map<BGRAColor>::SysType);
        }
        assert("Invalid element type" && false);
        return 0u;
    }

    class ENGINE_DLL Vertex {
        friend class VertexBuffer;

    public:
        template<ElementType Type>
        auto &Attr() noexcept;

        template<typename T>
        void SetAttributeByIndex(size_t i, T &&val) noexcept;

    protected:
        Vertex(char *pData, const VertexLayout &layout) noexcept;

    private:
        template<typename First, typename... Rest>
        // enables parameter pack setting of multiple parameters by element index
        void SetAttributeByIndex(size_t i, First &&first, Rest &&... rest) noexcept;

        // helper to reduce code duplication in SetAttributeByIndex
        template<ElementType DestLayoutType, typename SrcType>
        void SetAttribute(char *pAttribute, SrcType &&val) noexcept;

    private:
        char *pData = nullptr;
        const VertexLayout &layout;
    };

    template<ElementType Type>
    auto & Vertex::Attr() noexcept {
        auto pAttribute = pData + layout.Resolve<Type>().GetOffset();
        return *reinterpret_cast<typename Map<Type>::SysType *>(pAttribute);
    }

    template<typename T>
    void Vertex::SetAttributeByIndex(size_t i, T &&val) noexcept {
        const auto &element = layout.ResolveByIndex(i);
        auto pAttribute = pData + element.GetOffset();
        switch (element.GetType()) {
            case Position2D:
                SetAttribute<Position2D>(pAttribute, std::forward<T>(val));
                break;
            case Position3D:
                SetAttribute<Position3D>(pAttribute, std::forward<T>(val));
                break;
            case Texture2D:
                SetAttribute<Texture2D>(pAttribute, std::forward<T>(val));
                break;
            case Normal:
                SetAttribute<Normal>(pAttribute, std::forward<T>(val));
                break;
            case Float3Color:
                SetAttribute<Float3Color>(pAttribute, std::forward<T>(val));
                break;
            case Float4Color:
                SetAttribute<Float4Color>(pAttribute, std::forward<T>(val));
                break;
            case BGRAColor:
                SetAttribute<BGRAColor>(pAttribute, std::forward<T>(val));
                break;
            default:
                assert("Bad element type" && false);
        }
    }

    template<typename First, typename ... Rest>
    void Vertex::SetAttributeByIndex(size_t i, First &&first, Rest &&...rest) noexcept {
        SetAttributeByIndex(i, std::forward<First>(first));
        SetAttributeByIndex(i + 1, std::forward<Rest>(rest)...);
    }

    template<ElementType DestLayoutType, typename SrcType>
    void Vertex::SetAttribute(char *pAttribute, SrcType &&val) noexcept {
        using Dest = typename Map<DestLayoutType>::SysType;
        if constexpr (std::is_assignable<Dest, SrcType>::value) {
            *reinterpret_cast<Dest *>(pAttribute) = val;
        } else {
            assert("Parameter attribute type mismatch" && false);
        }
    }

    class ENGINE_DLL ConstVertex {
    public:
        ConstVertex(const Vertex &v) noexcept;

        template<ElementType Type>
        const auto &Attr() const noexcept;

    private:
        Vertex vertex;
    };

    template<ElementType Type>
    const auto & ConstVertex::Attr() const noexcept {
        return const_cast<Vertex &>(vertex).Attr<Type>();
    }

    class ENGINE_DLL VertexBuffer {
    public:
        VertexBuffer(VertexLayout layout) noexcept;

        const char *GetData() const noexcept;

        const VertexLayout &GetLayout() const noexcept;

        size_t Size() const noexcept;

        size_t SizeBytes() const noexcept;

        template<typename... Params>
        void EmplaceBack(Params &&... params) noexcept;

        Vertex Back() noexcept;

        Vertex Front() noexcept;

        Vertex operator[](size_t i) noexcept;

        ConstVertex Back() const noexcept;

        ConstVertex Front() const noexcept;

        ConstVertex operator[](size_t i) const noexcept;

    private:
        std::vector<char> buffer;
        VertexLayout layout;
    };

    template<typename ... Params>
    void VertexBuffer::EmplaceBack(Params &&...params) noexcept {
        assert(
            sizeof...(params) == layout.GetElementCount() && "Param count doesn't match number of vertex elements");
        buffer.resize(buffer.size() + layout.Size());
        Back().SetAttributeByIndex(0u, std::forward<Params>(params)...);
    }
}
