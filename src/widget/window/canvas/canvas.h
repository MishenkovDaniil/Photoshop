#ifndef CANVAS_H
#define CANVAS_H

#include "../../widget.h"
#include "../../button/button.h"

class Master_window;
struct Draw_tool 
{
    Button_type type = Unknown_button;
    Color color = Color (255, 0, 255, 255);
    bool is_pressed = false;
};

class Canvas : public Widget
{
    int width_ = 0;
    int height_ = 0;

    Color color_;
    Vector lh_pos_;
    Draw_tool draw_tool = {};

    sf::RenderTexture canvas_texture;
    sf::IntRect draw_rect_;

public: 
    Canvas (int width, int height, const Color color, const Vector lh_pos);
    ~Canvas ();

    void render                 (sf::RenderTarget &target)          override;
    bool on_mouse_pressed       (Mouse_key mouse_key, Vector &pos)  override;
    bool on_mouse_released      (Mouse_key mouse_key, Vector &pos)  override;
    bool on_mouse_moved         (Vector &new_pos)                   override;    /// x, y - absolute values 
    bool on_keyboard_pressed    (Keyboard_key key)                  override;
    bool on_keyboard_released   (Keyboard_key key)                  override;
    bool on_time                (float delta_sec)                   override;
    bool on_brush ( Mouse_key mouse_key, Vector &pos);

    bool contains (int x, int y);
    
    friend bool brush_button_act            (Master_window *m_window,  void *arg);
    friend bool change_canvas_rect_up_down  (Window *window,  void *arg);
    friend bool change_canvas_rect_mid      (Window *window,  void *arg);
    friend bool change_canvas_rect_space    (Window *window,  void *arg);

    friend Window;
};

#endif /* CANVAS_H */