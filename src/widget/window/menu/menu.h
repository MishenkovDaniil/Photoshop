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

    List buttons = {};
    // Transform transform_;
    
public:
    Menu (Vector lh_pos, int width, int height = MENU_HEIGHT);
    virtual ~Menu ();

    // Transform &get_transform () {return transform_;};

    void add_button (Button *button);
    
    void render (sf::RenderTarget &target, Transform_stack &transform_stack)    override;
    bool on_mouse_pressed     (Mouse_key mouse_key, Vector &pos, Transform_stack &transform_stack)                    override;
    bool on_mouse_released    (Mouse_key mouse_key, Vector &pos, Transform_stack &transform_stack)                    override;
    bool on_mouse_moved       (Vector &new_pos, Transform_stack &transform_stack)                                     override;    
    bool on_keyboard_pressed  (Keyboard_key key)                                    override;
    bool on_keyboard_released (Keyboard_key key)                                    override;
    bool on_time (float delta_sec)                                                  override;
};


#endif /* MENU_H */