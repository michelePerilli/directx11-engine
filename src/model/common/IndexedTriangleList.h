#pragma once
#include <vector>
#include <DirectXMath.h>


class ENGINE_DLL IndexedTriangleList
{
public:
	IndexedTriangleList() = default;
	IndexedTriangleList( Vertexes::VertexBuffer verts_in,std::vector<unsigned short> indices_in )
		:
		vertices( std::move( verts_in ) ),
		indices( std::move( indices_in ) )
	{
		assert( vertices.Size() > 2 );
		assert( indices.size() % 3 == 0 );
	}
	void Transform( DirectX::FXMMATRIX matrix )
	{
		using Elements = Vertexes::ElementType;
		for( int i = 0; i < vertices.Size(); i++ )
		{
			auto& pos = vertices[i].Attr<Elements::Position3D>();
			DirectX::XMStoreFloat3(
				&pos,
				DirectX::XMVector3Transform( DirectX::XMLoadFloat3( &pos ),matrix )
			);
		}
	}

public:
	Vertexes::VertexBuffer vertices;
	std::vector<unsigned short> indices;
};