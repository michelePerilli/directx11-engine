#include "../Drawable.h"
#include "../IndexBuffer.h"
#include <cassert>
#include <typeinfo>

void Drawable::Draw(Graphics &gfx) const noexcept {
    for( auto& b : binds )
    {
        b->Bind( gfx );
    }
    gfx.DrawIndexed( pIndexBuffer->GetCount() );
}

void Drawable::AddBind(std::shared_ptr<Bind::Bindable> bind) noexcept {
    // special case for index buffer
    if( typeid(*bind) == typeid(Bind::IndexBuffer) )
    {
        assert( "Binding multiple index buffers not allowed" && pIndexBuffer == nullptr );
        pIndexBuffer = &static_cast<Bind::IndexBuffer&>(*bind);
    }
    binds.push_back( std::move( bind ) );
}

