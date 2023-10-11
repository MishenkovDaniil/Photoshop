#include <iostream>
#include <cassert>

#include "window.h"

#include "../widget.h"
#include "menu/menu.h"
#include "header/header.h"
#include "canvas/canvas.h"
#include "scrollbar/scrollbar.h"

Window::Window (int width, int height, Vector lh_pos, const char *w_name, bool need_scrollbar) :
    width_ (width),
    height_ (height),
    lh_pos_ (lh_pos)
{
    header_ = new Header (lh_pos, width, w_name);
    canvas_ = new Canvas (width, height - HEADER_HEIGHT, Color (255, 255, 255, 255), Vector (lh_pos.get_x (), HEADER_HEIGHT  + lh_pos.get_y ()));
    assert (header_ && canvas_ && "failed to allocate window canvas and header\n");

    if (need_scrollbar)
    {
        scrollbar_ = new Scrollbar (lh_pos + Vector (width, HEADER_HEIGHT), height - HEADER_HEIGHT, canvas_->height_, height - HEADER_HEIGHT, this);
        assert (scrollbar_);
    }
}

Window::~Window ()
{
    width_  = __INT_MAX__; 
    height_ = __INT_MAX__;
    if (header_)    delete header_;
    if (canvas_)    delete canvas_;
    if (scrollbar_) delete scrollbar_;
}

void Window::render (sf::RenderTarget &target)
{
    sf::RectangleShape rect (Vector (width_, height_));
    rect.setFillColor (Color (0, 0,0,0));
    rect.setOutlineColor (Color (50, 50, 50));
    rect.setOutlineThickness (-1);
    rect.setPosition (lh_pos_);

    canvas_->render (target);
    if (scrollbar_) scrollbar_->render (target);
    header_->render (target);
    target.draw (rect);
}

bool Window::on_mouse_pressed  (Mouse_key mouse_key, Vector &pos)
{
    bool status = false;

    if (scrollbar_) 
    {
        status = scrollbar_->on_mouse_pressed (mouse_key, pos);
        if (status) return true;
    }
    
    status = canvas_->on_mouse_pressed (mouse_key, pos);
    if (status) return true;
    
    status = header_->on_mouse_pressed (mouse_key, pos);


    return status;
} 

bool Window::on_mouse_released (Mouse_key mouse_key, Vector &pos)
{
    bool status = false;

    if (scrollbar_) 
    {
        status = scrollbar_->on_mouse_released (mouse_key, pos);
        if (status) return true;
    }

    status = header_->on_mouse_released (mouse_key, pos);
    if (status) return true;
    
    status = canvas_->on_mouse_released (mouse_key, pos);
    
    return status;
} 

bool Window::on_mouse_moved    (Vector &new_pos)
{
    bool status = false;

    if (scrollbar_) 
    {
        status = scrollbar_->on_mouse_moved (new_pos);
        if (status) return true;
    }
    
    status = header_->on_mouse_moved (new_pos);
    if (status) return true;

    status = canvas_->on_mouse_moved (new_pos);
    
    return status;
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
    bool on_time_status = false;

    if (scrollbar_) 
    {
        on_time_status |= scrollbar_->on_time (delta_sec);
    }
    
    on_time_status |= header_->on_time (delta_sec);
    on_time_status |= canvas_->on_time (delta_sec);
    
    return on_time_status;
}
