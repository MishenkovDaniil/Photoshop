#ifndef PLUG_RENDERTARGET_H
#define PLUG_RENDERTARGET_H

#include "../graphic_structures/color/color.h"
#include "plug_vertex.h"
#include "plug_texture.h"

namespace plug
{
    class RenderTarget
    {
        virtual ~RenderTarget(void) = default;
        virtual void draw(const VertexArray& array) = 0;
        virtual void draw(const VertexArray& array, const Texture& texture) = 0;
        virtual void clear(Color color) = 0;
        virtual unsigned getNativeHandle(void) const = 0;
    };
}


#endif /* PLUG_RENDERTARGET_H */