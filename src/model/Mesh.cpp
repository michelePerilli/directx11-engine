#include "Mesh.h"
#include "../component/imgui/imgui.h"
#include <unordered_map>

// Mesh
Mesh::Mesh(Graphics &gfx, std::vector<std::unique_ptr<Bind::Bindable> > bindPtrs) {
    if (!IsStaticInitialized()) {
        AddStaticBind(std::make_unique<Bind::Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
    }

    for (auto &pb: bindPtrs) {
        if (auto pi = dynamic_cast<Bind::IndexBuffer *>(pb.get())) {
            AddIndexBuffer(std::unique_ptr<Bind::IndexBuffer>{pi});
            pb.release();
        } else {
            AddBind(std::move(pb));
        }
    }

    AddBind(std::make_unique<Bind::TransformCbuf>(gfx, *this));
}

void Mesh::Draw(Graphics &gfx, DirectX::FXMMATRIX accumulatedTransform) const noexcept {
    DirectX::XMStoreFloat4x4(&transform, accumulatedTransform);
    Drawable::Draw(gfx);
}

DirectX::XMMATRIX Mesh::GetTransformXM() const noexcept {
    return DirectX::XMLoadFloat4x4(&transform);
}


// Node
Node::Node(int id, const std::string &name, std::vector<Mesh *> meshPtrs, const DirectX::XMMATRIX &transform_in ) noexcept
    : id (id), meshPtrs(std::move(meshPtrs)), name(name) {
    DirectX::XMStoreFloat4x4(&transform, transform_in );
    DirectX::XMStoreFloat4x4(&appliedTransform, DirectX::XMMatrixIdentity());
}

void Node::Draw(Graphics &gfx, DirectX::FXMMATRIX accumulatedTransform) const noexcept {
    const auto built =
            DirectX::XMLoadFloat4x4(&appliedTransform) *
            DirectX::XMLoadFloat4x4(&transform) *
            accumulatedTransform;
    for (const auto pm: meshPtrs) {
        pm->Draw(gfx, built);
    }
    for (const auto &pc: childPtrs) {
        pc->Draw(gfx, built);
    }
}
void Node::AddChild(std::unique_ptr<Node> pChild) noexcept {
    assert(pChild);
    childPtrs.push_back(std::move(pChild));
}
void Node::ShowTree(Node *&pSelectedNode) const noexcept {

    const int selectedId = (pSelectedNode == nullptr) ? -1 : pSelectedNode->id;
    // build up flags for current node
    const auto node_flags = ImGuiTreeNodeFlags_OpenOnArrow
                            | (selectedId ? ImGuiTreeNodeFlags_Selected : 0)
                            | (childPtrs.empty() ? ImGuiTreeNodeFlags_Leaf : 0);
    bool expanded = ImGui::TreeNodeEx(reinterpret_cast<void *>(static_cast<intptr_t>(id)), node_flags,
                                       name.c_str());
    // if tree node expanded, recursively render all children
        if (ImGui::IsItemClicked()) {
            pSelectedNode = const_cast<Node *>(this);
        }
    if (expanded) {
        for (const auto &pChild: childPtrs) {
            pChild->ShowTree(pSelectedNode);
        }
        ImGui::TreePop();
    }
}

int Node::GetID() const noexcept {
    return id;
}


void Node::SetAppliedTransform(DirectX::FXMMATRIX transform) noexcept {
    DirectX::XMStoreFloat4x4(&appliedTransform, transform);
}

// Model
class ModelWindow // pImpl idiom, only defined in this .cpp
{
public:
    void Show(const char *windowName, const Node &root) noexcept {
        // window name defaults to "Model"
        windowName = windowName ? windowName : "Model";

        if (ImGui::Begin(windowName)) {
            ImGui::Columns(2, nullptr, true);
            root.ShowTree(pSelectedNode);

            ImGui::NextColumn();
            if (pSelectedNode != nullptr) {
                auto &transform = transforms[pSelectedNode->GetID()];
                ImGui::Text("Orientation");
                ImGui::SliderAngle("Roll", &transform.roll, -180.0f, 180.0f);
                ImGui::SliderAngle("Pitch", &transform.pitch, -180.0f, 180.0f);
                ImGui::SliderAngle("Yaw", &transform.yaw, -180.0f, 180.0f);
                ImGui::Text("Position");
                ImGui::SliderFloat("X", &transform.x, -20.0f, 20.0f);
                ImGui::SliderFloat("Y", &transform.y, -20.0f, 20.0f);
                ImGui::SliderFloat("Z", &transform.z, -20.0f, 20.0f);
            }
        }
        ImGui::End();
    }

    DirectX::XMMATRIX GetTransform() const noexcept {
        if (pSelectedNode == nullptr) {
            throw THROW_WINDOW_EXCEPTION("Nodo selezionato nullptr");
        }
        const auto &transform = transforms.at(pSelectedNode->GetID());
        return
                DirectX::XMMatrixRotationRollPitchYaw(transform.roll, transform.pitch, transform.yaw) *
                DirectX::XMMatrixTranslation(transform.x, transform.y, transform.z);
    }

    Node *GetSelectedNode() const noexcept {
        return pSelectedNode;
    }

private:
    Node *pSelectedNode;

    struct TransformParameters {
        float roll = 0.0f;
        float pitch = 0.0f;
        float yaw = 0.0f;
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;
    };

    std::unordered_map<int, TransformParameters> transforms;
};


Model::Model(Graphics &gfx, const std::string fileName)
    : pWindow(std::make_unique<ModelWindow>()) {
    Assimp::Importer imp;
    const auto pScene = imp.ReadFile(fileName.c_str(),
                                     aiProcess_Triangulate
                                     | aiProcess_JoinIdenticalVertices
                                     | aiProcess_ConvertToLeftHanded
                                     | aiProcess_GenNormals
    );

    if (pScene == nullptr) {
        throw Exception(__LINE__, __FILE__, imp.GetErrorString());
    }

    for (size_t i = 0; i < pScene->mNumMeshes; i++) {
        meshPtrs.push_back(ParseMesh(gfx, *pScene->mMeshes[i]));
    }
    int nextId = 0;
    pRoot = ParseNode(nextId, *pScene->mRootNode);
}

Model::~Model() noexcept {
}

void Model::Draw(Graphics &gfx) const {
    if( auto node = pWindow->GetSelectedNode() )
    {
        node->SetAppliedTransform( pWindow->GetTransform() );
    }
    pRoot->Draw( gfx,DirectX::XMMatrixIdentity() );
}

void Model::ShowWindow(const char *windowName) const noexcept {
    pWindow->Show(windowName, *pRoot);
}

std::unique_ptr<Mesh> Model::ParseMesh(Graphics &gfx, const aiMesh &mesh) {
    namespace dx = DirectX;

    Vertexes::VertexBuffer vbuf(std::move(
        Vertexes::VertexLayout{}
        .Append(Vertexes::Position3D)
        .Append(Vertexes::Normal)
    ));

    for (unsigned int i = 0; i < mesh.mNumVertices; i++) {
        vbuf.EmplaceBack(
            *reinterpret_cast<dx::XMFLOAT3 *>(&mesh.mVertices[i]),
            *reinterpret_cast<dx::XMFLOAT3 *>(&mesh.mNormals[i])
        );
    }

    std::vector<unsigned short> indices;
    indices.reserve(mesh.mNumFaces * 3);
    for (unsigned int i = 0; i < mesh.mNumFaces; i++) {
        const auto &face = mesh.mFaces[i];
        assert(face.mNumIndices == 3);
        indices.push_back(face.mIndices[0]);
        indices.push_back(face.mIndices[1]);
        indices.push_back(face.mIndices[2]);
    }

    std::vector<std::unique_ptr<Bind::Bindable> > bindablePtrs;

    bindablePtrs.push_back(std::make_unique<Bind::VertexBuffer>(gfx, vbuf));

    bindablePtrs.push_back(std::make_unique<Bind::IndexBuffer>(gfx, indices));

    auto pvs = std::make_unique<Bind::VertexShader>(gfx, L"D:/Dev/C++/gaming/GameEngine/shader/_PhongVS.cso");
    auto pvsbc = pvs->GetBytecode();
    bindablePtrs.push_back(std::move(pvs));

    bindablePtrs.push_back(
        std::make_unique<Bind::PixelShader>(gfx, L"D:/Dev/C++/gaming/GameEngine/shader/_PhongPS.cso"));

    bindablePtrs.push_back(std::make_unique<Bind::InputLayout>(gfx, vbuf.GetLayout().GetD3DLayout(), pvsbc));

    struct PSMaterialConstant {
        DirectX::XMFLOAT3 color = {0.6f, 0.6f, 0.8f};
        float specularIntensity = 0.6f;
        float specularPower = 30.0f;
        float padding[3];
    } pmc;
    bindablePtrs.push_back(std::make_unique<Bind::PixelConstantBuffer<PSMaterialConstant> >(gfx, pmc, 1u));

    return std::make_unique<Mesh>(gfx, std::move(bindablePtrs));
}

std::unique_ptr<Node> Model::ParseNode(int& nextId, const aiNode &node) {
    namespace dx = DirectX;
    const auto transform = dx::XMMatrixTranspose(dx::XMLoadFloat4x4(
        reinterpret_cast<const dx::XMFLOAT4X4 *>(&node.mTransformation)
    ));

    std::vector<Mesh *> curMeshPtrs;
    curMeshPtrs.reserve(node.mNumMeshes);
    for (size_t i = 0; i < node.mNumMeshes; i++) {
        const auto meshIdx = node.mMeshes[i];
        curMeshPtrs.push_back(meshPtrs.at(meshIdx).get());
    }

    auto pNode = std::make_unique<Node>(nextId++, node.mName.C_Str(), std::move(curMeshPtrs), transform);
    for (size_t i = 0; i < node.mNumChildren; i++) {
        pNode->AddChild(ParseNode(nextId, *node.mChildren[i]));
    }

    return pNode;
}
