#include <iostream>
#include <cassert>

#include "window.h"

#include "../widget.h"
#include "menu/menu.h"
#include "header/header.h"
#include "canvas/canvas.h"
#include "scrollbar/scrollbar.h"

Window::Window (int width, int height, Vector lh_pos, const char *w_name, bool need_scrollbar, Tool_palette *palette) :
    transform_ (Transform (lh_pos)),
    width_ (width),
    height_ (height)
    // lh_pos_ (lh_pos)
{
    header_ = new Header (Vector (0, 0), width, w_name);
    canvas_ = new Canvas (width, height - HEADER_HEIGHT, Color (255, 255, 255, 255), Vector (0, HEADER_HEIGHT), palette);
    assert (header_ && canvas_ && "failed to allocate window canvas and header\n");

    if (need_scrollbar)
    {
        scrollbar_ = new Scrollbar (Vector (width, HEADER_HEIGHT), height - HEADER_HEIGHT, canvas_->height_, height - HEADER_HEIGHT, this);
        assert (scrollbar_ && "failed to allocate window scrollbar \n");
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

void Window::render (sf::RenderTarget &target, M_vector<Transform> &transform_stack)
{
    Transform top = transform_stack.get_size () > 0 ? transform_stack.top () : Transform (Vector (0, 0));
    Transform unite = transform_.unite (top);
    transform_stack.push (unite);


    Vector lh_pos = transform_stack.top ().offset_;

    sf::RectangleShape rect (Vector (width_, height_));
    rect.setFillColor (Color (0, 0,0,0));
    rect.setOutlineColor (Color (50, 50, 50));
    rect.setOutlineThickness (-1);
    rect.setPosition (lh_pos);

    // transform_stack.push (canvas_->transform_);
    canvas_->render (target, transform_stack);
    // transform_stack.pop ();

    if (scrollbar_) 
    {
        // transform_stack.push (scrollbar_->transform_);
        scrollbar_->render (target, transform_stack);
        // transform_stack.pop ();
    }
    
    // transform_stack.push (header_->transform_);
    header_->render (target, transform_stack);
    // transform_stack.pop ();
    
    target.draw (rect);

    transform_stack.pop ();
}

bool Window::on_mouse_pressed  (Mouse_key mouse_key, Vector &pos, M_vector<Transform> &transform_stack)
{
    Transform top = transform_stack.get_size () > 0 ? transform_stack.top () : Transform (Vector (0, 0));
    Transform unite = transform_.unite (top);
    transform_stack.push (unite);
    
    bool status = false;

    if (scrollbar_) 
    {
        status = scrollbar_->on_mouse_pressed (mouse_key, pos, transform_stack);
        if (status) 
        {
            transform_stack.pop ();
            return true;
        }
    }
    
    status = canvas_->on_mouse_pressed (mouse_key, pos, transform_stack);
    if (status) 
    {
        transform_stack.pop ();
        return true;
    }
    
    status = header_->on_mouse_pressed (mouse_key, pos, transform_stack);

    transform_stack.pop ();

    return status;
} 

bool Window::on_mouse_released (Mouse_key mouse_key, Vector &pos, M_vector<Transform> &transform_stack)
{
    Transform top = transform_stack.get_size () > 0 ? transform_stack.top () : Transform (Vector (0, 0));
    Transform unite = transform_.unite (top);
    transform_stack.push (unite);

    bool status = false;

    if (scrollbar_) 
    {
        status = scrollbar_->on_mouse_released (mouse_key, pos, transform_stack);
        if (status) 
        {
            transform_stack.pop ();
            return true;
        }
    }

    status = header_->on_mouse_released (mouse_key, pos, transform_stack);
    if (status) 
    {
        transform_stack.pop ();
        return true;
    }
    
    status = canvas_->on_mouse_released (mouse_key, pos, transform_stack);
    
    transform_stack.pop ();

    return status;
} 

bool Window::on_mouse_moved    (Vector &new_pos, M_vector<Transform> &transform_stack)
{
    Transform top = transform_stack.get_size () > 0 ? transform_stack.top () : Transform (Vector (0, 0));
    Transform unite = transform_.unite (top);
    transform_stack.push (unite);

    bool status = false;

    if (scrollbar_) 
    {
        status = scrollbar_->on_mouse_moved (new_pos, transform_stack);
        if (status) 
        {
            transform_stack.pop ();
            return true;
        }
    }
    
    status = header_->on_mouse_moved (new_pos, transform_stack);
    if (status) 
    {
        transform_stack.pop ();
        return true;
    }

    status = canvas_->on_mouse_moved (new_pos, transform_stack);

    transform_stack.pop ();

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
