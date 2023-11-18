#include <iostream>
#include <cassert>

#include "window.h"

#include "../widget.h"
#include "menu/menu.h"
#include "header/header.h"
#include "canvas/canvas.h"
#include "scrollbar/scrollbar.h"

Window::Window (int width, int height, Vec2d lh_pos, const char *w_name, bool need_scrollbar, Tool_palette *palette) :
    width_ (width),
    height_ (height),
    contained_widgets (nullptr, 3)
{
    layout_= new Default_layout_box (lh_pos, Vec2d (width, height));

    header_ = new Header (Vec2d (0, 0), width, w_name, this);
    canvas_ = new Canvas (width, height - HEADER_HEIGHT, Color (255, 255, 255, 255), Vec2d (0, HEADER_HEIGHT), palette);
    assert (header_ && canvas_ && "failed to allocate window canvas and header\n");

    if (need_scrollbar)
    {
        scrollbar_ = new Scrollbar (Vec2d (width, HEADER_HEIGHT), height - HEADER_HEIGHT, canvas_->height_, height - HEADER_HEIGHT, this);
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

void Window::render (sf::RenderTarget &target, TransformStack &transform_stack)
{
    transform_stack.enter (Transform (layout_->get_position ()));

    Vec2d lh_pos = transform_stack.top ().getOffset ();

    sf::RectangleShape rect (Vec2d (width_, height_));
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

    transform_stack.leave ();
}



void Window::onMousePressed     ( MousePressedEvent &event, EHC &ehc)
{
    ehc.stack.enter (Transform (layout_->get_position ()));

    size_t widgets_num = contained_widgets.get_size ();

    for (size_t window_widget_idx = 0; window_widget_idx < widgets_num; ++window_widget_idx)
    {
        if (contained_widgets[window_widget_idx])
        {
            contained_widgets[window_widget_idx]->onMousePressed (event, ehc);
            if (ehc.stopped == true)
            {
                ehc.stack.leave ();
                return;
            }
        }
    }

    ehc.stack.leave ();
}

void Window::onMouseReleased    ( MouseReleasedEvent &event, EHC &ehc)
{
    ehc.stack.enter (Transform (layout_->get_position ()));

    size_t widgets_num = contained_widgets.get_size ();
    for (size_t window_widget_idx = 0; window_widget_idx < widgets_num; ++window_widget_idx)
    {
        if (contained_widgets[window_widget_idx]) 
            contained_widgets[window_widget_idx]->onMouseReleased (event, ehc);
    }

    ehc.stack.leave ();
}

void Window::onMouseMove        ( MouseMoveEvent &event, EHC &ehc)
{
    ehc.stack.enter (Transform (layout_->get_position ()));

    size_t widgets_num = contained_widgets.get_size ();
    for (size_t window_widget_idx = 0; window_widget_idx < widgets_num; ++window_widget_idx)
    {
        if (contained_widgets[window_widget_idx]) 
        {
            contained_widgets[window_widget_idx]->onMouseMove (event, ehc);
            if (ehc.stopped == true)
            {
                ehc.stack.leave ();
                return;
            }
        }
    }

    ehc.stack.leave ();
}

void Window::onKeyboardPressed  ( KeyboardPressedEvent &event, EHC &ehc)
{
    size_t widgets_num = contained_widgets.get_size ();

    for (size_t window_widget_idx = 0; window_widget_idx < widgets_num; ++window_widget_idx)
    {
        if (contained_widgets[window_widget_idx])
        {
            contained_widgets[window_widget_idx]->onKeyboardPressed (event, ehc);
            if (ehc.stopped)
                return;
        }
    }
} 

void Window::onKeyboardReleased ( KeyboardReleasedEvent &event, EHC &ehc)
{
    size_t widgets_num = contained_widgets.get_size ();
    
    for (size_t window_widget_idx = 0; window_widget_idx < widgets_num; ++window_widget_idx)
    {
        if (contained_widgets[window_widget_idx])
            contained_widgets[window_widget_idx]->onKeyboardReleased (event, ehc);
    }
}

void Window::onTick             ( TickEvent &event, EHC &ehc)
{
    size_t widgets_num = contained_widgets.get_size ();
    for (size_t window_widget_idx = 0; window_widget_idx < widgets_num; ++window_widget_idx)
    {
        if (contained_widgets[window_widget_idx])
        {
            contained_widgets[window_widget_idx]->onTick (event, ehc);
        }
    }
}

bool Window::contains (const Vec2d &pos)
{   
    Vec2d pos_ = pos - layout_->get_position ();

    double x = pos_.get_x ();
    double y = pos_.get_y ();

    if ((x >= 0 && y >= 0) && 
        (x <= width_ && y <= height_))
        return true;

    return false;  
} 