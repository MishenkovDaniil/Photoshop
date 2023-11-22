#ifndef PLUG_CANVAS_H
#define PLUG_CANVAS_H


#include <SFML/Graphics.hpp>
#include "../graphic_structures/vector/vector.h"
#include "plug_vertex.h"
#include "plug_texture.h"
#include "plug_widget.h"

struct SelectionMask;

namespace plug
{
    class Canvas
    {
    public:
        virtual ~Canvas () = default;
        virtual bool contains (int x, int y) = 0;
        
        virtual Vec2d get_size () = 0;
        virtual sf::IntRect &get_draw_rect () = 0;
        virtual void set_draw_rect_offset (int left, int top) = 0;
        virtual void set_draw_rect_size   (int width, int height) = 0;
        virtual void setSelectionMask (SelectionMask &new_selection) = 0;
        virtual const SelectionMask &getSelectionMask () = 0;

        virtual sf::RenderTexture &getRenderTexture () = 0;
    };
    //Class Canvas
//   virtual ~Canvas() = default;
//   virtual void draw(const VertexArray& vertex_array) = 0;
//   virtual void draw(const VertexArray& vertex_array,
//                     const Texture&     texture) = 0;
//   virtual unsigned int getNativeHandle(void) const = 0;
//   virtual Vec2d getSize(void) const = 0;
//   virtual void setSize(const Vec2d& size) = 0;
//   virtual SelectionMask& getSelectionMask(void) = 0;
//   virtual Color getPixel(size_t x, size_t y) const = 0;
//   virtual void setPixel(size_t x, size_t y, const Color& color) = 0;
//   virtual const Texture& getTexture(void) const = 0;
}

#endif /* PLUG_CANVAS_H */