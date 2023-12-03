#ifndef MENU_H
#define MENU_H

#include <cassert>
#include <iostream>

#include "../../../list.h"
#include "../../widget.h"
#include "../../button/button.h"
#include "../../../graphics/rectangleshape/rectangleshape.h"

static const int MENU_HEIGHT = 20;
static const int MENU_INIT_CAPACITY = 10;

class Button;
class Window;
class Master_window;

class Menu : public Widget 
{
protected:
    int width_ = 0;
    int cur_width_ = 0;
    int height_ = 0;

    M_vector<Button *> buttons = M_vector<Button *> (nullptr);
    
public:
    Menu (plug::Vec2d lh_pos, int width, int height = MENU_HEIGHT);
    virtual ~Menu ();

    void add_button (Button *button);
    
    void draw (plug::TransformStack &transform_stack, plug::RenderTarget &target) override;
    void onTick             (const plug::TickEvent &event, plug::EHC &ehc) override;
    void onMouseMove        (const plug::MouseMoveEvent &event, plug::EHC &ehc) override;
    void onMousePressed     (const plug::MousePressedEvent &event, plug::EHC &ehc) override;
    void onMouseReleased    (const plug::MouseReleasedEvent &event, plug::EHC &ehc) override;
    void onKeyboardPressed  (const plug::KeyboardPressedEvent &event, plug::EHC &ehc) override;
    void onKeyboardReleased (const plug::KeyboardReleasedEvent &event, plug::EHC &ehc) override;
};

#endif /* MENU_H */