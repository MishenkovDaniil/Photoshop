#ifndef MENU_H
#define MENU_H

#include <cassert>
#include <iostream>

class Button;
class Window;
class Master_window;
#include "../../../list.h"
#include "../../widget.h"
#include "../../button/button.h"

static const int MENU_HEIGHT = 20;
static const int MENU_INIT_CAPACITY = 10;

class Menu : public Widget 
{
protected:
    int width_ = 0;
    int cur_width_ = 0;
    int height_ = 0;

    M_vector<Button *> buttons = M_vector<Button *> (nullptr);
    
public:
    Menu (Vec2d lh_pos, int width, int height = MENU_HEIGHT);
    virtual ~Menu ();

    void add_button (Button *button);
    
    void render (sf::RenderTarget &target, TransformStack &transform_stack)                        override;
    void onTick             (const TickEvent &event, EHC &ehc) override;
    void onMouseMove        (const MouseMoveEvent &event, EHC &ehc) override;
    void onMousePressed     (const MousePressedEvent &event, EHC &ehc) override;
    void onMouseReleased    (const MouseReleasedEvent &event, EHC &ehc) override;
    void onKeyboardPressed  (const KeyboardPressedEvent &event, EHC &ehc) override;
    void onKeyboardReleased (const KeyboardReleasedEvent &event, EHC &ehc) override;
};

#endif /* MENU_H */