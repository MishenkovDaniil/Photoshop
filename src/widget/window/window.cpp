#include <iostream>
#include <cassert>

#include "window.h"

#include "../widget.h"
#include "menu/menu.h"
#include "header/header.h"
#include "canvas/canvas.h"
#include "scrollbar/scrollbar.h"

Window::Window (int width, int height, Vector lh_pos, const char *w_name, bool need_scrollbar, Tool_palette *palette) :
    width_ (width),
    height_ (height),
    contained_widgets (nullptr, 3)
{
    layout_= new Default_layout_box (lh_pos, Vector (width, height));

    header_ = new Header (Vector (0, 0), width, w_name, this);
    canvas_ = new Canvas (width, height - HEADER_HEIGHT, Color (255, 255, 255, 255), Vector (0, HEADER_HEIGHT), palette);
    assert (header_ && canvas_ && "failed to allocate window canvas and header\n");

    if (need_scrollbar)
    {
        scrollbar_ = new Scrollbar (Vector (width, HEADER_HEIGHT), height - HEADER_HEIGHT, canvas_->height_, height - HEADER_HEIGHT, this);
        assert (scrollbar_ && "failed to allocate window scrollbar \n");

        Widget *scrollbar = scrollbar_;
        contained_widgets.add (scrollbar);
    }

    Widget *header = header_;
    Widget *canvas = canvas_;
    contained_widgets.add (header);
    contained_widgets.add (canvas);
}

Window::~Window ()
{
    width_  = __INT_MAX__; 
    height_ = __INT_MAX__;

    if (header_)    delete header_;
    if (canvas_)    delete canvas_;
    if (scrollbar_) delete scrollbar_;
    delete layout_;
}

void Window::render (sf::RenderTarget &target, Transform_stack &transform_stack)
{
    transform_stack.push (Transform (layout_->get_position ()));

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

bool Window::on_mouse_pressed  (Mouse_key mouse_key, Vector &pos, Transform_stack &transform_stack)
{   
    transform_stack.push (Transform (layout_->get_position ()));

    size_t widgets_num = contained_widgets.get_size ();

    for (size_t window_widget_idx = 0; window_widget_idx < widgets_num; ++window_widget_idx)
    {
        if (contained_widgets[window_widget_idx] && contained_widgets[window_widget_idx]->on_mouse_pressed (mouse_key, pos, transform_stack))
        {
            transform_stack.pop ();
            return true;
        }
    }

    transform_stack.pop ();
    return false;
} 

bool Window::on_mouse_released (Mouse_key mouse_key, Vector &pos, Transform_stack &transform_stack)
{
    bool status = false;
    
    transform_stack.push (Transform (layout_->get_position ()));

    size_t widgets_num = contained_widgets.get_size ();

    for (size_t window_widget_idx = 0; window_widget_idx < widgets_num; ++window_widget_idx)
    {
        status |= (contained_widgets[window_widget_idx] && contained_widgets[window_widget_idx]->on_mouse_released (mouse_key, pos, transform_stack));
    }

    transform_stack.pop ();
    return status;
} 

bool Window::on_mouse_moved    (Vector &new_pos, Transform_stack &transform_stack)
{
    transform_stack.push (Transform (layout_->get_position ()));

    size_t widgets_num = contained_widgets.get_size ();

    for (size_t window_widget_idx = 0; window_widget_idx < widgets_num; ++window_widget_idx)
    {
        if (contained_widgets[window_widget_idx] && contained_widgets[window_widget_idx]->on_mouse_moved (new_pos, transform_stack))
        {
            transform_stack.pop ();
            return true;
        }
    }

    transform_stack.pop ();
    return false;
}   

bool Window::on_keyboard_pressed  (Keyboard_key key)
{
    size_t widgets_num = contained_widgets.get_size ();

    for (size_t window_widget_idx = 0; window_widget_idx < widgets_num; ++window_widget_idx)
    {
        if (contained_widgets[window_widget_idx] && contained_widgets[window_widget_idx]->on_keyboard_pressed (key))
        {
            return true;
        }
    }

    return false;
} 

bool Window::on_keyboard_released (Keyboard_key key)
{
    size_t widgets_num = contained_widgets.get_size ();
    bool status = false;
    for (size_t window_widget_idx = 0; window_widget_idx < widgets_num; ++window_widget_idx)
    {
        if (contained_widgets[window_widget_idx] && contained_widgets[window_widget_idx]->on_keyboard_released (key))
        {
            status = true;
        }
    }

    return status;
} 

bool Window::on_tick (float delta_sec)
{
    bool status = false;

    size_t widgets_num = contained_widgets.get_size ();
    for (size_t window_widget_idx = 0; window_widget_idx < widgets_num; ++window_widget_idx)
    {
        if (contained_widgets[window_widget_idx] && contained_widgets[window_widget_idx]->on_tick (delta_sec))
        {
            status = true;
        }
    }
    
    return status;
}


bool Window::contains (Vector &pos)
{   
    Vector pos_ = pos - layout_->get_position ();

    double x = pos_.get_x ();
    double y = pos_.get_y ();

    if ((x >= 0 && y >= 0) && 
        (x <= width_ && y <= height_))
        return true;

    return false;  
} 