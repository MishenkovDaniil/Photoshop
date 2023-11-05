#ifndef TEXTURE_WIDGET_H
#define TEXTURE_WIDGET_H

#include "../widget.h"

class M_render_texture : public Widget
{
public:
    sf::RenderTexture texture_;
    M_render_texture (int width, int height, Color color)  {texture_.create (width, height); texture_.clear (color);};
    ~M_render_texture () = default;

    void clear (Color color) {texture_.clear (color);};
    void render (sf::RenderTarget &target, Transform_stack &transform_stack); 
    bool on_mouse_pressed  (Mouse_key mouse_key, Vector &pos, Transform_stack &transform_stack) {return false;};
    bool on_mouse_released (Mouse_key mouse_key, Vector &pos, Transform_stack &transform_stack) {return false;};
    bool on_mouse_moved    (Vector &new_pos, Transform_stack &transform_stack) {return false;};    /// x, y - absolute values 
    bool on_keyboard_pressed  (Keyboard_key key) {return false;};
    bool on_keyboard_released (Keyboard_key key) {return false;};
    bool on_time (float delta_sec) {return false;};
};

#endif /* TEXTURE_WIDGET_H */