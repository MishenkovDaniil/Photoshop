#ifndef WINDOW_H
#define WINDOW_H

class Canvas;
class Master_window;
class Header;
class Scrollbar;

#include "../widget.h"
#include "menu/menu.h"
#include "header/header.h"
#include "canvas/canvas.h"
#include "tools/palette/palette.h"
// #include "scrollbar/scrollbar.h"

/// TODO : FRAME


class Window : public Widget
{
protected:
    int width_  = 0;
    int height_ = 0;
    
    Canvas    *canvas_      = nullptr;
    Header    *header_      = nullptr;
    Scrollbar *scrollbar_   = nullptr;

    M_vector<Widget *> contained_widgets;
    
    // Transform transform_;
    
public:
    Window (int width, int height, Vec2d lh_pos, const char *w_name, bool need_scrollbar = true, Tool_palette *palette = nullptr);
    virtual ~Window ();

    Canvas *get_canvas () {return canvas_;};

    void render (sf::RenderTarget &target, TransformStack &transform_stack)  override;
    void onTick             (TickEvent &event, EHC &ehc) override;
    void onMouseMove        (MouseMoveEvent &event, EHC &ehc) override;
    void onMousePressed     (MousePressedEvent &event, EHC &ehc) override;
    void onMouseReleased    (MouseReleasedEvent &event, EHC &ehc) override;
    void onKeyboardPressed  (KeyboardPressedEvent &event, EHC &ehc) override;
    void onKeyboardReleased (KeyboardReleasedEvent &event, EHC &ehc) override;


    bool contains (const Vec2d &pos);
    
    friend bool change_canvas_rect_up_down  (void *window,  void *arg);
    friend bool change_canvas_rect_mid      (void *window,  void *arg);
    friend bool change_canvas_rect_space    (void *window,  void *arg);
    friend void init_canvases (Master_window *m_window, Tool_palette *palette);

    friend Scrollbar;
};


#endif /* WINDOW_H */
//TODO:make 2 constructors: one with window parameters only and default canvas and menu parameters (calc by window width and height)
        //  another with user all parameters