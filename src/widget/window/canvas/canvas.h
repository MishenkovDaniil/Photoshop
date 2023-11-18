#ifndef CANVAS_H
#define CANVAS_H

class Tool;
class Tool_palette;
#include "../../widget.h"
#include "../../button/button.h"
#include "../tools/tools.h"
#include "../tools/palette/palette.h"

class Master_window;

class Canvas : public Widget
{
    int width_ = 0;
    int height_ = 0;

    Color color_;

    Tool_palette *palette_;
    sf::IntRect draw_rect_;
    bool is_focused = false;
    
public:
    sf::RenderTexture canvas_texture;

public: 
    Canvas (int width, int height, const Color color, const Vec2d lh_pos, Tool_palette *palette = nullptr);
    ~Canvas ();

    void render                 (sf::RenderTarget &target, TransformStack &transform_stack)    override;
    void onTick             (TickEvent &event, EHC &ehc) override;
    void onMouseMove        (MouseMoveEvent &event, EHC &ehc) override;
    void onMousePressed     (MousePressedEvent &event, EHC &ehc) override;
    void onMouseReleased    (MouseReleasedEvent &event, EHC &ehc) override;
    void onKeyboardPressed  (KeyboardPressedEvent &event, EHC &ehc) override;
    void onKeyboardReleased (KeyboardReleasedEvent &event, EHC &ehc) override;


    bool contains (int x, int y);
    
    Color get_fg_color ();
    Color get_bg_color ();  
    Vec2d get_size () {return Vec2d (width_, height_);};
    sf::IntRect &get_draw_rect () {return draw_rect_;};
    void set_draw_rect_offset (int left, int top);
    void set_draw_rect_size   (int width, int height);
    
    friend bool change_canvas_rect_up_down  (void *window, void *arg);
    friend bool change_canvas_rect_mid      (void *window, void *arg);
    friend bool change_canvas_rect_space    (void *window, void *arg);
    friend void init_canvases (Master_window *m_window, Tool_palette *palette);

    friend Window;
    friend Tool;
};

#endif /* CANVAS_H */