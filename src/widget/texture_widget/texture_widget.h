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
    void render (sf::RenderTarget &target, TransformStack &transform_stack); 
    // bool on_mouse_pressed  (MouseButton mouse_button, Vec2d &pos, TransformStack &transform_stack) {return false;};
    // bool on_mouse_released (MouseButton mouse_button, Vec2d &pos, TransformStack &transform_stack) {return false;};
    // bool on_mouse_moved    (Vec2d &new_pos, TransformStack &transform_stack) {return false;};    /// x, y - absolute values 
    // bool on_keyboard_pressed  (KeyCode key) {return false;};
    // bool on_keyboard_released (KeyCode key) {return false;};
    // bool on_tick (float delta_sec) {return false;};
    void onTick             (TickEvent &event, EHC &ehc) {return;};
    void onMouseMove        (MouseMoveEvent &event, EHC &ehc) {return;};
    void onMousePressed     (MousePressedEvent &event, EHC &ehc) {return;};
    void onMouseReleased    (MouseReleasedEvent &event, EHC &ehc) {return;};
    void onKeyboardPressed  (KeyboardPressedEvent &event, EHC &ehc) {return;};
    void onKeyboardReleased (KeyboardReleasedEvent &event, EHC &ehc) {return;};
};

#endif /* TEXTURE_WIDGET_H */