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

    plug::Color fill_color_ = plug::Color (200, 200, 200);
    plug::Color outline_color_ = plug::Color (50, 50, 50);

    M_vector<Button *> buttons = M_vector<Button *> (nullptr);
    
public:
    Menu (plug::Vec2d lh_pos, int width, int height = MENU_HEIGHT);
    virtual ~Menu ();

    void add_button (Button *button);
    size_t getButtonNum () {return buttons.get_size ();};
    virtual Button *popButton () {assert(buttons.get_size ()); Button *button = buttons.top (); buttons.pop ();};

    void setFillColor (const plug::Color color) {fill_color_ = color;};
    void setOutlineColor (const plug::Color color) {outline_color_ = color;};

    void draw (plug::TransformStack &transform_stack, plug::RenderTarget &target) override;
    void onTick             (const plug::TickEvent &event, plug::EHC &ehc) override;
    void onMouseMove        (const plug::MouseMoveEvent &event, plug::EHC &ehc) override;
    void onMousePressed     (const plug::MousePressedEvent &event, plug::EHC &ehc) override;
    void onMouseReleased    (const plug::MouseReleasedEvent &event, plug::EHC &ehc) override;
    void onKeyboardPressed  (const plug::KeyboardPressedEvent &event, plug::EHC &ehc) override;
    void onKeyboardReleased (const plug::KeyboardReleasedEvent &event, plug::EHC &ehc) override;
};

#endif /* MENU_H */