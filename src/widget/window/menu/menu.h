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
    // bool on_mouse_pressed     (MouseButton mouse_button, Vec2d &pos, TransformStack &transform_stack)  override;
    // bool on_mouse_released    (MouseButton mouse_button, Vec2d &pos, TransformStack &transform_stack)  override;
    // bool on_mouse_moved       (Vec2d &new_pos, TransformStack &transform_stack)                   override;    
    // bool on_keyboard_pressed  (KeyCode key)                                                    override;
    // bool on_keyboard_released (KeyCode key)                                                    override;
    // bool on_tick (float delta_sec)                                                                  override;

    void onTick             (TickEvent &event, EHC &ehc) override;
    void onMouseMove        (MouseMoveEvent &event, EHC &ehc) override;
    void onMousePressed     (MousePressedEvent &event, EHC &ehc) override;
    void onMouseReleased    (MouseReleasedEvent &event, EHC &ehc) override;
    void onKeyboardPressed  (KeyboardPressedEvent &event, EHC &ehc) override;
    void onKeyboardReleased (KeyboardReleasedEvent &event, EHC &ehc) override;
};

#endif /* MENU_H */