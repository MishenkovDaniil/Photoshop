#ifndef WINDOW_H
#define WINDOW_H

#include "../widget.h"
#include "menu/menu.h"
#include "header/header.h"
#include "canvas/canvas.h"
#include "../../plugin/tools/palette/palette.h"

class CanvasView;
class Master_window;
class Header;
class Scrollbar; 
class Tool_palette;  

class Window : public Widget
{
protected:
    int width_  = 0;
    int height_ = 0;
    
    CanvasView    *canvas_      = nullptr;
    Header    *header_      = nullptr;
    Scrollbar *scrollbar_   = nullptr;

    M_vector<Widget *> contained_widgets;
    Master_window *parent_window_ = nullptr;
    
public:
    Window (int width, int height, plug::Vec2d lh_pos, const char *w_name, bool need_scrollbar = true, Tool_palette *palette = nullptr);
    virtual ~Window ();

    CanvasView *get_canvas () {return canvas_;};

    void draw (plug::TransformStack &transform_stack, plug::RenderTarget &target)  override;
    void onTick             (const plug::TickEvent &event, plug::EHC &ehc) override;
    void onMouseMove        (const plug::MouseMoveEvent &event, plug::EHC &ehc) override;
    void onMousePressed     (const plug::MousePressedEvent &event, plug::EHC &ehc) override;
    void onMouseReleased    (const plug::MouseReleasedEvent &event, plug::EHC &ehc) override;
    void onKeyboardPressed  (const plug::KeyboardPressedEvent &event, plug::EHC &ehc) override;
    void onKeyboardReleased (const plug::KeyboardReleasedEvent &event, plug::EHC &ehc) override;

    void set_parent (Master_window *parent_window) {parent_window_ = parent_window;};

    friend bool change_canvas_rect_up_down  (void *window,  void *arg);
    friend bool change_canvas_rect_mid      (void *window,  void *arg);
    friend bool change_canvas_rect_space    (void *window,  void *arg);
    friend void init_canvases (Master_window *m_window, Tool_palette *palette);

    friend Scrollbar;
};

#endif /* WINDOW_H */