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
    Vector move_start_;

public: 
    Header (Vector lh_pos, int width, const char *string, Window *window, Color background = Color (150, 150, 150));
    ~Header ();

    bool contains (int x, int y);

    void render (sf::RenderTarget &target, Transform_stack &transform_stack)    override;
    bool on_mouse_pressed     (Mouse_key mouse_key, Vector &pos, Transform_stack &transform_stack)                    override;
    bool on_mouse_released    (Mouse_key mouse_key, Vector &pos, Transform_stack &transform_stack)                    override;
    bool on_mouse_moved       (Vector &new_pos, Transform_stack &transform_stack)                                     override;   
    bool on_keyboard_pressed  (Keyboard_key key)                                    override;
    bool on_keyboard_released (Keyboard_key key)                                    override;
    bool on_time (float delta_sec)                                                  override;

    friend Window;
};


#endif /* HEADER_H */