#ifndef CANVAS_H
#define CANVAS_H

class Tool;
class Tool_palette;
#include "../../widget.h"
#include "../../button/button.h"
#include "../tools/tools.h"
#include "../tools/palette/palette.h"

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
    // Vector lh_pos_;
    Draw_tool draw_tool = {};

    // sf::RenderTexture buffer_texture; ?? need to be window size for preview 
    sf::IntRect draw_rect_;
    Tool_palette *palette_;
    Transform transform_;

public:
    sf::RenderTexture canvas_texture;

public: 
    Canvas (int width, int height, const Color color, const Vector lh_pos, Tool_palette *palette = nullptr);
    ~Canvas ();

    void render                 (sf::RenderTarget &target, M_vector<Transform> &transform_stack)          override;
    bool on_mouse_pressed       (Mouse_key mouse_key, Vector &pos)  override;
    bool on_mouse_released      (Mouse_key mouse_key, Vector &pos)  override;
    bool on_mouse_moved         (Vector &new_pos)                   override;    /// x, y - absolute values 
    bool on_keyboard_pressed    (Keyboard_key key)                  override;
    bool on_keyboard_released   (Keyboard_key key)                  override;
    bool on_time                (float delta_sec)                   override;
    bool on_brush ( Mouse_key mouse_key, Vector &pos);

    bool contains (int x, int y);
    
    Color get_fg_color ();
    Color get_bg_color ();

    friend bool brush_button_act            (void *m_window,  void *arg);
    friend bool change_canvas_rect_up_down  (void *window,  void *arg);
    friend bool change_canvas_rect_mid      (void *window,  void *arg);
    friend bool change_canvas_rect_space    (void *window,  void *arg);
    friend void init_canvases (Master_window *m_window, Tool_palette *palette);

    friend Window;
    friend Tool;
};

#endif /* CANVAS_H */