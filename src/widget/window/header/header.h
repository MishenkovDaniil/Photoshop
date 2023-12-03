#ifndef HEADER_H 
#define HEADER_H

#include <iostream>
#include <cassert>

#include "../../widget.h"
#include "../window.h"
#include "../../constants.h"
#include "../../../standard/plug_layout.h"
#include "../../../graphics/rectangleshape/rectangleshape.h"
#include "../../../graphics/rendertexture/rendertexture.h"
#include "../../../graphics/text/text.h"
#include "../../../graphics/font/font.h"

static const int HEADER_HEIGHT       = 30;
static const int CHARACTER_SIZE      = 20;

class Window;

class Header : public Widget
{
    char *string_ = nullptr;
    int width_ = 0;
    int str_len = 0;
    
    const char *font_file_ = DEFAULT_FONT_FILE;

    plug::Color background_color;
    Window *parent_window_ = nullptr;
    
    bool is_moving_ = false;
    plug::Vec2d move_start_;

public: 
    Header (plug::Vec2d lh_pos, int width, const char *string, Window *window, plug::Color background = plug::Color (150, 150, 150));
    ~Header ();

    void draw (plug::TransformStack &transform_stack, plug::RenderTarget &target)    override;
    void onTick             (const plug::TickEvent &event, plug::EHC &ehc) override;
    void onMouseMove        (const plug::MouseMoveEvent &event, plug::EHC &ehc) override;
    void onMousePressed     (const plug::MousePressedEvent &event, plug::EHC &ehc) override;
    void onMouseReleased    (const plug::MouseReleasedEvent &event, plug::EHC &ehc) override;
    void onKeyboardPressed  (const plug::KeyboardPressedEvent &event, plug::EHC &ehc) override;
    void onKeyboardReleased (const plug::KeyboardReleasedEvent &event, plug::EHC &ehc) override;

    friend Window;
};

#endif /* HEADER_H */