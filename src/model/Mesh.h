#pragma once
#include "../component/pipeline/DrawableBase.h"
#include "../component/pipeline/BindableBase.h"
#include "../component/pipeline/Vertex.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class ENGINE_DLL Mesh : public DrawableBase<Mesh> {
public:
    Mesh(Graphics &gfx, std::vector<std::unique_ptr<Bind::Bindable> > bindPtrs);

    void Draw(Graphics &gfx, DirectX::FXMMATRIX accumulatedTransform) const noexcept;

    DirectX::XMMATRIX GetTransformXM() const noexcept override;

private:
    mutable DirectX::XMFLOAT4X4 transform;
};

class ENGINE_DLL Node {
    friend class Model;

public:
    Node(std::vector<Mesh *> meshPtrs, const DirectX::XMMATRIX &transform) noexcept;

    void Draw(Graphics &gfx, DirectX::FXMMATRIX accumulatedTransform) const noexcept;

private:
    void AddChild(std::unique_ptr<Node> pChild) noexcept;

private:
    std::vector<std::unique_ptr<Node> > childPtrs;
    std::vector<Mesh *> meshPtrs;
    DirectX::XMFLOAT4X4 transform;
};

class ENGINE_DLL Model {
public:
    Model(Graphics &gfx, const std::string fileName);

    void Draw(Graphics &gfx, DirectX::FXMMATRIX transform) const;

private:
    static std::unique_ptr<Mesh> ParseMesh(Graphics &gfx, const aiMesh &mesh);

    std::unique_ptr<Node> ParseNode(const aiNode &node);

private:
    std::unique_ptr<Node> pRoot;
    std::vector<std::unique_ptr<Mesh> > meshPtrs;
};
