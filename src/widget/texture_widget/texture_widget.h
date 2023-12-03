#ifndef TEXTURE_WIDGET_H
#define TEXTURE_WIDGET_H

#include "../widget.h"

#include "../../graphics/rendertexture/rendertexture.h"

class M_render_texture : public Widget
{
    bool is_changed_ = false;
public:
    RenderTexture texture_;
    Sprite cur_sprite;

    M_render_texture (int width, int height, plug::Color color)  {texture_.create (width, height); texture_.clear (color); cur_sprite.setPosition (0, 0);};
    ~M_render_texture () = default;

    void draw (plug::TransformStack &transform_stack, plug::RenderTarget &target); 
    void onTick             (const plug::TickEvent &event, plug::EHC &ehc) {return;};
    void onMouseMove        (const plug::MouseMoveEvent &event, plug::EHC &ehc) {return;};
    void onMousePressed     (const plug::MousePressedEvent &event, plug::EHC &ehc) {return;};
    void onMouseReleased    (const plug::MouseReleasedEvent &event, plug::EHC &ehc) {return;};
    void onKeyboardPressed  (const plug::KeyboardPressedEvent &event, plug::EHC &ehc) {return;};
    void onKeyboardReleased (const plug::KeyboardReleasedEvent &event, plug::EHC &ehc) {return;};
    
    plug::Texture getTexture () const {return texture_.getTexture ();};

    void draw (const plug::VertexArray& array) {is_changed_ = true; texture_.draw (array);};
    void draw (const Drawable &drawable) {is_changed_ = true; texture_.draw (drawable);};
    void draw (const plug::VertexArray& array, const plug::Texture& texture) {is_changed_ = true; texture_.draw (array, texture);};
    
    void clear (plug::Color color) {is_changed_ = true; texture_.clear (color);};
private:
    void init_vertices (plug::VertexArray &array, const plug::Texture *texture);
};

#endif /* TEXTURE_WIDGET_H */