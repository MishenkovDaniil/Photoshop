#ifndef PLUG_CANVAS_H
#define PLUG_CANVAS_H

#include "../graphic_structures/vector/vector.h"
#include "plug_vertex.h"
#include "plug_texture.h"
#include "plug_widget.h"
#include "plug_selection_mask.h"

namespace plug
{
    class Canvas
    {
    public:
        virtual ~Canvas () = default;
        
        virtual Vec2d getSize () const = 0;
        virtual void setSize(const Vec2d& size) = 0;
        virtual const SelectionMask &getSelectionMask () = 0;
        virtual Color getPixel(size_t x, size_t y) const = 0;
        virtual void setPixel(size_t x, size_t y, const Color& color) = 0;
        virtual const Texture& getTexture(void) const = 0;

        virtual unsigned int getNativeHandle(void) const = 0;             

        virtual void draw(const VertexArray& vertex_array) = 0;             
        virtual void draw(const VertexArray& vertex_array, const Texture &texture) = 0;
    };
}

#endif /* PLUG_CANVAS_H */