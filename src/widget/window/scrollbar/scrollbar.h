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
static const int UP_DOWN_CANVAS_CHANGE = 10;

class Scrollbar : public Button
{
    M_vector<Button *> buttons = M_vector<Button *> ((Button *)nullptr);
    Button *up_    = nullptr;
    Button *down_  = nullptr;
    Button *mid_   = nullptr;


    int scrollbar_height_ = 0;
    double shift = 0;

public: 
    Scrollbar (Vector lh_pos, int height, int obj_height, int obj_allowed_height, Window *window);
    ~Scrollbar ();

    void render (sf::RenderTarget &target, M_vector<Transform> &transform_stack)    override;
    bool on_mouse_pressed     (Mouse_key mouse_key, Vector &pos, M_vector<Transform> &transform_stack)                    override;
    bool on_mouse_released    (Mouse_key mouse_key, Vector &pos, M_vector<Transform> &transform_stack)                    override;
    bool on_mouse_moved       (Vector &new_pos, M_vector<Transform> &transform_stack)                                     override;    
    bool on_keyboard_pressed  (Keyboard_key key)                                    override;
    bool on_keyboard_released (Keyboard_key key)                                    override;
    bool on_time (float delta_sec)                                                  override;

    friend Window;
};

bool change_canvas_rect_up_down (void *window, void *arg);
bool change_canvas_rect_mid     (void *window, void *arg);
bool change_canvas_rect_space   (void *window, void *arg);
bool change_canvas_rect_up      (void *window, void *arg);
bool change_canvas_rect_down    (void *window, void *arg);

#endif /* SCROLLBAR_H */
