#include "TestAssimpObject.h"
#include "../component/pipeline/BindableBase.h"
#include "../interface/Graphics.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../component/pipeline/Vertex.h"

TestAssimpObject::TestAssimpObject(
    Graphics &gfx,
    std::mt19937 &rng,
    std::uniform_real_distribution<float> &adist,
    std::uniform_real_distribution<float> &ddist,
    std::uniform_real_distribution<float> &odist,
    std::uniform_real_distribution<float> &rdist,
    DirectX::XMFLOAT3 material,
    float scale) : TestObject(gfx, rng, adist, ddist, odist, rdist) {
    namespace dx = DirectX;

    if (!IsStaticInitialized()) {
        using hw3dexp::VertexLayout;
        hw3dexp::VertexBuffer vbuf(std::move(
            VertexLayout{}
            .Append(hw3dexp::Position3D)
            .Append(hw3dexp::Normal)
        ));

        Assimp::Importer importer;
        const auto pModel = importer.ReadFile("D:/Dev/C++/gaming/GameEngine/assets/models/suzanne.obj", aiProcess_JoinIdenticalVertices);
        const auto pMesh = pModel->mMeshes[0];

        for (unsigned int i = 0; i < pMesh->mNumVertices; i++) {
            vbuf.EmplaceBack(
                dx::XMFLOAT3{
                    pMesh->mVertices[i].x * scale, pMesh->mVertices[i].y * scale, pMesh->mVertices[i].z * scale
                },
                *reinterpret_cast<dx::XMFLOAT3 *>(&pMesh->mNormals[i])
            );
        }

        std::vector<unsigned short> indices;
        indices.reserve(pMesh->mNumFaces * 3);
        for (unsigned int i = 0; i < pMesh->mNumFaces; i++) {
            const auto &face = pMesh->mFaces[i];
            assert(face.mNumIndices == 3);
            indices.push_back(face.mIndices[0]);
            indices.push_back(face.mIndices[1]);
            indices.push_back(face.mIndices[2]);
        }

        AddStaticBind(std::make_unique<VertexBuffer>(gfx, vbuf));

        AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, indices));

        auto pvs = std::make_unique<VertexShader>(gfx, L"D:/Dev/C++/gaming/GameEngine/shader/_PhongVS.cso");
        auto pvsbc = pvs->GetBytecode();
        AddStaticBind(std::move(pvs));

        AddStaticBind(std::make_unique<PixelShader>(gfx, L"D:/Dev/C++/gaming/GameEngine/shader/_PhongPS.cso"));

        AddStaticBind(std::make_unique<InputLayout>(gfx, vbuf.GetLayout().GetD3DLayout(), pvsbc));

        AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

        struct PSMaterialConstant {
            DirectX::XMFLOAT3 color;
            float specularIntensity = 0.6f;
            float specularPower = 30.0f;
            float padding[3];
        } pmc;
        pmc.color = material;
        AddStaticBind(std::make_unique<PixelConstantBuffer<PSMaterialConstant> >(gfx, pmc, 1u));
    } else {
        SetIndexFromStatic();
    }
    AddBind(std::make_unique<TransformCbuf>(gfx, *this));
}
