#ifndef SCROLLBAR_H
#define SCROLLBAR_H

#include <cassert>
#include <iostream>

// class Button;
#include "../../widget.h"
#include "../canvas/canvas.h"
#include "../../button/button.h"

static const int SCROLLBAR_WIDTH = 20;
static const int SCROLLBAR_BUTTON_H = 10;
static const double UP_BUTTON_CHANGE = 0.1;
static const double SPACE_BUTTON_CHANGE = 0.2;

class Scrollbar : public Button
{
    Vector lh_pos_;
    Button *up_    = nullptr;
    Button *down_  = nullptr;
    Button *mid_   = nullptr;
    // Button *space_ = nullptr;

    int scrollbar_height_ = 0;
    double shift = 0;
    int obj_height_ = 0;
    int obj_allowed_height_ = 0;
    bool mid_pressed_ = false;

public: 
    Scrollbar (Vector lh_pos, int height, int obj_height, int obj_allowed_height, Window *window);
    ~Scrollbar ();

    void render (sf::RenderTarget &target) override;
    bool on_mouse_pressed  (Mouse_key mouse_key, Vector &pos) override;
    bool on_mouse_released (Mouse_key mouse_key, Vector &pos) override;
    bool on_mouse_moved    (Vector &new_pos) override;    /// x, y - absolute values 
    bool on_keyboard_pressed  (Keyboard_key key) override;
    bool on_keyboard_released (Keyboard_key key) override;
    bool on_time (float delta_sec) override;
};

bool change_canvas_rect_up_down (Window *window, void *arg);
bool change_canvas_rect_mid     (Window *window, void *arg);
bool change_canvas_rect_space   (Window *window, void *arg);
bool change_canvas_rect_up      (Window *window, void *arg);
bool change_canvas_rect_down    (Window *window, void *arg);

#endif /* SCROLLBAR_H */
