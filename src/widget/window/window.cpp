#include <iostream>
#include <cassert>

#include "window.h"


Window::Window (int width, int height, Vector lh_pos, const char *w_name) :
    width_ (width),
    height_ (height),
    lh_pos_ (lh_pos)
{
    header_ = new Header (lh_pos, width, w_name);
    canvas_ = new Canvas (width, height - HEADER_HEIGHT, Color (255, 255, 255, 255), Vector (lh_pos.get_x (), HEADER_HEIGHT  + lh_pos.get_y ()));
    assert (header_ && canvas_ && "failed to allocate window canvas and header\n");
}

Window::~Window ()
{
    width_  = __INT_MAX__; 
    height_ = __INT_MAX__;
    if (header_) delete header_;
    if (canvas_) delete canvas_;
}

void Window::render (sf::RenderTarget &target) const
{
    canvas_->render (target);
    header_->render (target);
}

bool Window::on_mouse_pressed  (Mouse_key mouse_key, Vector &pos)
{
    bool canvas_status = canvas_->on_mouse_pressed (mouse_key, pos);
    bool header_status = header_->on_mouse_pressed (mouse_key, pos);
    return canvas_status | header_status;
} 

bool Window::on_mouse_released (Mouse_key mouse_key, Vector &pos)
{
    bool canvas_status = canvas_->on_mouse_released (mouse_key, pos);
    bool header_status = header_->on_mouse_released (mouse_key, pos);
    return canvas_status | header_status;
} 

bool Window::on_mouse_moved    (Vector &new_pos)
{
    //TODO
    return false;
}   

bool Window::on_keyboard_pressed  (Keyboard_key key)
{
    //TODO
    return false;
} 

bool Window::on_keyboard_released (Keyboard_key key)
{
    //TODO
    return false;
} 

bool Window::on_time (float delta_sec)
{
    //TODO
    return false;
}
