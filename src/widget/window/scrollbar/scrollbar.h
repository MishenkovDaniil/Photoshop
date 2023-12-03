#ifndef SCROLLBAR_H
#define SCROLLBAR_H

#include <cassert>
#include <iostream>

// class Button;
#include "../../widget.h"
#include "../canvas/canvas.h"
#include "../../button/button.h"
#include "../../../standard/plug_transform.h"
#include "../../../standard/plug_layout.h"

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
    Scrollbar (plug::Vec2d lh_pos, int height, int obj_height, int obj_allowed_height, Window *window);
    ~Scrollbar ();

    void draw (plug::TransformStack &transform_stack, plug::RenderTarget &target)    override;
    void onTick             (const plug::TickEvent &event, plug::EHC &ehc) override;
    void onMouseMove        (const plug::MouseMoveEvent &event, plug::EHC &ehc) override;
    void onMousePressed     (const plug::MousePressedEvent &event, plug::EHC &ehc) override;
    void onMouseReleased    (const plug::MouseReleasedEvent &event, plug::EHC &ehc) override;
    void onKeyboardPressed  (const plug::KeyboardPressedEvent &event, plug::EHC &ehc) override;
    void onKeyboardReleased (const plug::KeyboardReleasedEvent &event, plug::EHC &ehc) override;

    friend Window;
};

bool change_canvas_rect_up_down (void *window, void *arg);
bool change_canvas_rect_mid     (void *window, void *arg);
bool change_canvas_rect_space   (void *window, void *arg);
bool change_canvas_rect_up      (void *window, void *arg);
bool change_canvas_rect_down    (void *window, void *arg);

#endif /* SCROLLBAR_H */
