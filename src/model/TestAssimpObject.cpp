#include "TestAssimpObject.h"

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"

TestAssimpObject::TestAssimpObject(
    Graphics &gfx,
    std::mt19937 &rng,
    std::uniform_real_distribution<float> &adist,
    std::uniform_real_distribution<float> &ddist,
    std::uniform_real_distribution<float> &odist,
    std::uniform_real_distribution<float> &rdist,
    DirectX::XMFLOAT3 material,
    float scale): TestObject(gfx, rng, adist, ddist, odist, rdist) {
    if (!IsStaticInitialized()) {
        struct Vertex {
            DirectX::XMFLOAT3 pos; // position
            DirectX::XMFLOAT3 n; // normal?
        };

        Assimp::Importer importer;
        const auto pModel = importer.ReadFile("D:/Dev/C++/gaming/GameEngine/assets/models/suzanne.obj", aiProcess_JoinIdenticalVertices);
        const auto pMesh = pModel->mMeshes[0];
        std::vector<Vertex> vertices;
        vertices.reserve(pMesh->mNumVertices);
        for (unsigned int i = 0; i < pMesh->mNumVertices; i++) {
            vertices.push_back(
                {
                    {
                        pMesh->mVertices[i].x * scale,
                        pMesh->mVertices[i].y * scale,
                        pMesh->mVertices[i].z * scale
                    },
                    *reinterpret_cast<DirectX::XMFLOAT3 *>(&pMesh->mNormals[i])
                }
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

        AddStaticBind(std::make_unique<VertexBuffer>(gfx, vertices));
        AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, indices));

        auto psv = std::make_unique<VertexShader>(gfx, L"D:/Dev/C++/gaming/GameEngine/shader/_PhongVS.cso");
        auto pvsbc = psv->GetBytecode();
        AddStaticBind(std::move(psv));
        AddStaticBind(std::make_unique<PixelShader>(gfx, L"D:/Dev/C++/gaming/GameEngine/shader/_PhongPS.cso"));

        const std::vector<D3D11_INPUT_ELEMENT_DESC> inputElementDesc = {

            {"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"Normal", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA, 0},
        };

        AddStaticBind(std::make_unique<InputLayout>(gfx, inputElementDesc, pvsbc));
        AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ));

        struct PSMateriaConstant {
            DirectX::XMFLOAT3 color;
            float specularIntensity = 0.6f;
            float specularPower = 30.0f;
            float padding[3];
        } pmc;
        pmc.color = material;
        AddStaticBind(std::make_unique<PixelConstantBuffer<PSMateriaConstant>>(gfx, pmc, 1));
    } else {
        SetIndexFromStatic();
    }
    AddBind( std::make_unique<TransformCbuf>( gfx,*this ) );
}
