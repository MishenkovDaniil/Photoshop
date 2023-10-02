#ifndef WINDOW_H
#define WINDOW_H

#include "../widget.h"
#include "canvas/canvas.h"

class Window : public Widget
{
    int width_ = 0;
    int height_ = 0;
    Vector lh_pos_;
    Canvas canvas;
    // int thickness_ = 0;          ///
                                    /// frame decorator
    // const Color &frame_color_;   /// 

    // const Color &w_color_;       ///canvas
    
public: 
    //TODO:make 2 constructors: one with window parameters only and default canvas and menu parameters (calc by window width and height)
                            //  another with user all parameters
    Window (int width, int height, Vector lh_pos, int canvas_width, int canvas_height, const Color canvas_color, const Vector canvas_lh_pos) :
            width_ (width),
            height_ (height),
            lh_pos_ (lh_pos),
            canvas (canvas_width, canvas_height, canvas_color, canvas_lh_pos)
            {};

    virtual ~Window () {width_  = __INT_MAX__; 
                        height_ = __INT_MAX__;};

    void render (sf::RenderTarget &target) const override;
    void render (sf::RenderTarget &target) const override;
    bool on_mouse_pressed  (Mouse_key mouse_key, Vector &pos) override;
    bool on_mouse_released (Mouse_key mouse_key, Vector &pos) override;
    bool on_mouse_moved    (Vector &new_pos) override;    /// x, y - absolute values 
    bool on_keyboard_pressed  (Keyboard_key key) override;
    bool on_keyboard_released (Keyboard_key key) override;
    bool on_time (float delta_sec);
};


#endif /* WINDOW_H */