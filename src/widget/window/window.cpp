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
{
    header_ = new Header (Vector (0, 0), width, w_name, this);
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
    // printf ("window transform = %lf %lf\n", transform_.offset_.get_x (), transform_.offset_.get_y ());
    Vector lh_pos = transform_stack.top ().offset_;

    sf::RectangleShape rect (Vector (width_, height_));
    rect.setFillColor (Color (0, 0,0,0));
    rect.setOutlineColor (Color (50, 50, 50));
    rect.setOutlineThickness (-1);
    rect.setPosition (lh_pos);

    canvas_->render (target, transform_stack);

    if (scrollbar_) 
    {
        scrollbar_->render (target, transform_stack);
    }
    
    header_->render (target, transform_stack);
    
    target.draw (rect);

    transform_stack.pop ();
}

bool Window::on_mouse_pressed  (Mouse_key mouse_key, Vector &pos, M_vector<Transform> &transform_stack)
{   
    Transform top = transform_stack.get_size () > 0 ? transform_stack.top () : Transform (Vector (0, 0));
    Transform unite = transform_.unite (top);
    transform_stack.push (unite);

    if (scrollbar_ && scrollbar_->on_mouse_pressed (mouse_key, pos, transform_stack))
    {
        transform_stack.pop ();
        return true;
    }
    if (header_->on_mouse_pressed (mouse_key, pos, transform_stack))
    {
        transform_stack.pop ();
        return true;
    }
    
    if (canvas_->on_mouse_pressed (mouse_key, pos, transform_stack))
    {
        transform_stack.pop ();
        return true;
    }
    transform_stack.pop ();
    return false;
} 

bool Window::on_mouse_released (Mouse_key mouse_key, Vector &pos, M_vector<Transform> &transform_stack)
{
    Transform top = transform_stack.get_size () > 0 ? transform_stack.top () : Transform (Vector (0, 0));
    Transform unite = transform_.unite (top);
    transform_stack.push (unite);

    if (scrollbar_ && scrollbar_->on_mouse_released (mouse_key, pos, transform_stack))
    {
        transform_stack.pop ();
        return true;
    }
    if (header_->on_mouse_released (mouse_key, pos, transform_stack))
    {
        transform_stack.pop ();
        return true;
    }
    if (canvas_->on_mouse_released (mouse_key, pos, transform_stack))
    {
        transform_stack.pop ();
        return true;
    }

    transform_stack.pop ();
    return false;
} 

bool Window::on_mouse_moved    (Vector &new_pos, M_vector<Transform> &transform_stack)
{
    Transform top = transform_stack.get_size () > 0 ? transform_stack.top () : Transform (Vector (0, 0));
    Transform unite = transform_.unite (top);
    transform_stack.push (unite);

    if (scrollbar_ && scrollbar_->on_mouse_moved (new_pos, transform_stack))
    {
        transform_stack.pop ();
        return true;
    }
    
    if (header_->on_mouse_moved (new_pos, transform_stack))
    {
        transform_stack.pop ();
        return true;
    }

    if (canvas_->on_mouse_moved (new_pos, transform_stack))
    {
        transform_stack.pop ();
        return true;
    }

    transform_stack.pop ();
    return false;
}   

bool Window::on_keyboard_pressed  (Keyboard_key key)
{
    if (scrollbar_ && scrollbar_->on_keyboard_pressed (key)) return true;
    if (header_->on_keyboard_pressed (key)) return true;

    return canvas_->on_keyboard_pressed (key);
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


bool Window::contains (Vector &pos)
{   
    Vector pos_ = transform_.apply_transform (pos);
    
    double x = pos_.get_x ();
    double y = pos_.get_y ();

    if ((x >= 0 && y >= 0) && 
        (x <= width_ && y <= height_))
        return true;

    return false;  
} 