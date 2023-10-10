#ifndef HEADER_H 
#define HEADER_H

#include <iostream>
#include <cassert>

#include "../../widget.h"
#include "../window.h"

static const int HEADER_HEIGHT       = 30;
static const int CHARACTER_SIZE      = 20;
static const Color TEXT_COLOR        = Color (0, 0, 0);
static const char *DEFAULT_FONT_FILE = "/home/daniil/programming/code/C++_projects/photoshop/resources/font.ttf";

class Header : public Widget
{
    Vector lh_pos_;
    char *string_ = nullptr;
    int width_ = 0;
    int str_len = 0;
    const int height_ = HEADER_HEIGHT;
    Color background_color;
    const char *font_file_ = DEFAULT_FONT_FILE;

public: 
    Header (Vector lh_pos, int width, const char *string, Color background = Color (150, 150, 150));
    ~Header ();

    void render (sf::RenderTarget &target) override;
    bool on_mouse_pressed  (Mouse_key mouse_key, Vector &pos) override;
    bool on_mouse_released (Mouse_key mouse_key, Vector &pos) override;
    bool on_mouse_moved    (Vector &new_pos) override;   
    bool on_keyboard_pressed  (Keyboard_key key) override;
    bool on_keyboard_released (Keyboard_key key) override;
    bool on_time (float delta_sec) override;

    friend Window;
};


#endif /* HEADER_H */