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
// #include "scrollbar/scrollbar.h"

class Window : public Widget
{
protected:
    int width_  = 0;
    int height_ = 0;
    Vector lh_pos_;
    // int thickness_ =  0;         ///
                                    /// frame decorator
    // const Color &frame_color_;   /// 
    
    Canvas    *canvas_      = nullptr;
    Header    *header_      = nullptr;
    Scrollbar *scrollbar_   = nullptr;

public:
    Window (int width, int height, Vector lh_pos, const char *w_name, bool need_scrollbar = false);
    virtual ~Window ();

    void render (sf::RenderTarget &target)  override;
    bool on_mouse_pressed  (Mouse_key mouse_key, Vector &pos) override;
    bool on_mouse_released (Mouse_key mouse_key, Vector &pos) override;
    bool on_mouse_moved    (Vector &new_pos) override;    /// x, y - absolute values 
    bool on_keyboard_pressed  (Keyboard_key key) override;
    bool on_keyboard_released (Keyboard_key key) override;
    bool on_time (float delta_sec) override;

    friend bool brush_button_act (Master_window *m_window, void *arg);
    friend bool change_canvas_rect_up_down  (Window *window,  void *arg);
    friend bool change_canvas_rect_mid      (Window *window,  void *arg);
    friend bool change_canvas_rect_space    (Window *window,  void *arg);

    friend Scrollbar;
};


#endif /* WINDOW_H */
//TODO:make 2 constructors: one with window parameters only and default canvas and menu parameters (calc by window width and height)
        //  another with user all parameters