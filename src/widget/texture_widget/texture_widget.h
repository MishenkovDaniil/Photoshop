#ifndef TEXTURE_WIDGET_H
#define TEXTURE_WIDGET_H

#include "../widget.h"

#include "../../graphics/rendertexture/rendertexture.h"

class M_render_texture : public Widget
{
public:
    RenderTexture texture_;
    Sprite cur_sprite;

    M_render_texture (int width, int height, plug::Color color)  {texture_.create (width, height); texture_.clear (color); cur_sprite.setPosition (0, 0);};
    ~M_render_texture () = default;

    void render (plug::RenderTarget &target, plug::TransformStack &transform_stack); 
    void onTick             (const plug::TickEvent &event, plug::EHC &ehc) {return;};
    void onMouseMove        (const plug::MouseMoveEvent &event, plug::EHC &ehc) {return;};
    void onMousePressed     (const plug::MousePressedEvent &event, plug::EHC &ehc) {return;};
    void onMouseReleased    (const plug::MouseReleasedEvent &event, plug::EHC &ehc) {return;};
    void onKeyboardPressed  (const plug::KeyboardPressedEvent &event, plug::EHC &ehc) {return;};
    void onKeyboardReleased (const plug::KeyboardReleasedEvent &event, plug::EHC &ehc) {return;};
    void draw (const plug::VertexArray& array) {texture_.draw (array, cur_sprite);};
    void draw (const Drawable &drawable) {texture_.draw (drawable, cur_sprite);};
    void draw (const plug::VertexArray& array, const plug::Texture& texture) {texture_.draw (array, texture, cur_sprite);};
    void clear (plug::Color color) {texture_.clear (color, cur_sprite);};
};

#endif /* TEXTURE_WIDGET_H */