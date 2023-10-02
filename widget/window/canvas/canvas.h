#ifndef CANVAS_H
#define CANVAS_H

#include "../../widget.h"
#include "../../button/button.h"


struct Draw_tool 
{
    Button_type type = Unknown_button;
    Color color;
    bool is_pressed = false;
};

class Canvas : public Widget
{
    int width_ = 0;
    int height_ = 0;

    Color color_;
    Vector lh_pos_;
    Draw_tool draw_tool = {};
    sf::Texture canvas_texture;

public: 
    Canvas (int width, int height, const Color color, const Vector lh_pos);
    ~Canvas ();
    void render                 (sf::RenderTarget &target)    const override final;
    bool on_mouse_pressed       (Mouse_key mouse_key, Vector &pos)  override final;
    bool on_mouse_released      (Mouse_key mouse_key, Vector &pos)  override final;
    bool on_mouse_moved         (Vector &new_pos)                   override final;    /// x, y - absolute values 
    bool on_keyboard_pressed    (Keyboard_key key)                  override final;
    bool on_keyboard_released   (Keyboard_key key)                  override final;
    bool on_time                (float delta_sec)                   override final;
    bool on_brush ( Mouse_key mouse_key, Vector &pos);
};

#endif /* CANVAS_H */