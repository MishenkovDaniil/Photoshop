#ifndef HEADER_H 
#define HEADER_H

#include <iostream>
#include <cassert>

#include "../../widget.h"
#include "../window.h"
#include "../../constants.h"

static const int HEADER_HEIGHT       = 30;
static const int CHARACTER_SIZE      = 20;

class Header : public Widget
{
    char *string_ = nullptr;
    int width_ = 0;
    int str_len = 0;
    
    const char *font_file_ = DEFAULT_FONT_FILE;

    Color background_color;
    // Transform transform_;
    Window *parent_window_ = nullptr;
    
    bool is_moving_ = false;
    Vec2d move_start_;

public: 
    Header (Vec2d lh_pos, int width, const char *string, Window *window, Color background = Color (150, 150, 150));
    ~Header ();

    bool contains (int x, int y);

    void render (sf::RenderTarget &target, TransformStack &transform_stack)    override;
    void onTick             (const TickEvent &event, EHC &ehc) override;
    void onMouseMove        (const MouseMoveEvent &event, EHC &ehc) override;
    void onMousePressed     (const MousePressedEvent &event, EHC &ehc) override;
    void onMouseReleased    (const MouseReleasedEvent &event, EHC &ehc) override;
    void onKeyboardPressed  (const KeyboardPressedEvent &event, EHC &ehc) override;
    void onKeyboardReleased (const KeyboardReleasedEvent &event, EHC &ehc) override;

    friend Window;
};


#endif /* HEADER_H */