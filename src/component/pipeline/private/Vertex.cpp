#include "../Vertex.h"

Vertexes::VertexLayout::Element::Element(ElementType type, size_t offset) noexcept: type(type),
                                                                          offset(offset)  {
}

size_t Vertexes::VertexLayout::Element::GetOffsetAfter() const noexcept {
    return offset + Size();
}

size_t Vertexes::VertexLayout::Element::GetOffset() const {
    return offset;
}

size_t Vertexes::VertexLayout::Element::Size() const noexcept {
    return SizeOf(type);
}

Vertexes::ElementType Vertexes::VertexLayout::Element::GetType() const noexcept {
    return type;
}

D3D11_INPUT_ELEMENT_DESC Vertexes::VertexLayout::Element::GetDesc() const noexcept {
    switch (type) {
        case Position2D:
            return GenerateDesc<Position2D>(GetOffset());
        case Position3D:
            return GenerateDesc<Position3D>(GetOffset());
        case Texture2D:
            return GenerateDesc<Texture2D>(GetOffset());
        case Normal:
            return GenerateDesc<Normal>(GetOffset());
        case Float3Color:
            return GenerateDesc<Float3Color>(GetOffset());
        case Float4Color:
            return GenerateDesc<Float4Color>(GetOffset());
        case BGRAColor:
            return GenerateDesc<BGRAColor>(GetOffset());
    }
    assert("Invalid element type" && false);
    return {"INVALID", 0, DXGI_FORMAT_UNKNOWN, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0};
}

const Vertexes::VertexLayout::Element & Vertexes::VertexLayout::ResolveByIndex(size_t i) const noexcept {
    return elements[i];
}

Vertexes::VertexLayout & Vertexes::VertexLayout::Append(ElementType type) noexcept {
    elements.emplace_back(type, Size());
    return *this;
}

size_t Vertexes::VertexLayout::Size() const noexcept {
    return elements.empty() ? 0u : elements.back().GetOffsetAfter();
}

size_t Vertexes::VertexLayout::GetElementCount() const noexcept {
    return elements.size();
}

std::vector<D3D11_INPUT_ELEMENT_DESC> Vertexes::VertexLayout::GetD3DLayout() const noexcept {
    std::vector<D3D11_INPUT_ELEMENT_DESC> desc;
    desc.reserve(GetElementCount());
    for (const auto &e: elements) {
        desc.push_back(e.GetDesc());
    }
    return desc;
}

Vertexes::Vertex::Vertex(char *pData, const VertexLayout &layout) noexcept: pData(pData),
                                                                           layout(layout) {
    assert(pData != nullptr);
}

Vertexes::ConstVertex::ConstVertex(const Vertex &v) noexcept: vertex(v) {
}

Vertexes::VertexBuffer::VertexBuffer(VertexLayout layout) noexcept: layout(std::move(layout)) {
}

const char * Vertexes::VertexBuffer::GetData() const noexcept {
    return buffer.data();
}

const Vertexes::VertexLayout & Vertexes::VertexBuffer::GetLayout() const noexcept {
    return layout;
}

size_t Vertexes::VertexBuffer::Size() const noexcept {
    return buffer.size() / layout.Size();
}

size_t Vertexes::VertexBuffer::SizeBytes() const noexcept {
    return buffer.size();
}

Vertexes::Vertex Vertexes::VertexBuffer::Back() noexcept {
    assert(buffer.size() != 0u);
    return Vertex{buffer.data() + buffer.size() - layout.Size(), layout};
}

Vertexes::Vertex Vertexes::VertexBuffer::Front() noexcept {
    assert(buffer.size() != 0u);
    return Vertex{buffer.data(), layout};
}

Vertexes::Vertex Vertexes::VertexBuffer::operator[](size_t i) noexcept {
    assert(i < Size());
    return Vertex{buffer.data() + layout.Size() * i, layout};
}

Vertexes::ConstVertex Vertexes::VertexBuffer::Back() const noexcept {
    return const_cast<VertexBuffer *>(this)->Back();
}

Vertexes::ConstVertex Vertexes::VertexBuffer::Front() const noexcept {
    return const_cast<VertexBuffer *>(this)->Front();
}

Vertexes::ConstVertex Vertexes::VertexBuffer::operator[](size_t i) const noexcept {
    return const_cast<VertexBuffer &>(*this)[i];
}
