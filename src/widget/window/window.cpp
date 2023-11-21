#include <iostream>
#include <cassert>

#include "window.h"

#include "../widget.h"
#include "menu/menu.h"
#include "header/header.h"
#include "canvas/canvas.h"
#include "scrollbar/scrollbar.h"

plug::Window::Window (int width, int height, plug::Vec2d lh_pos, const char *w_name, bool need_scrollbar, Tool_palette *palette) :
    width_ (width),
    height_ (height),
    contained_widgets (nullptr, 3)
{
    layout_= new plug::Default_layout_box (lh_pos, Vec2d (width, height));

    header_ = new plug::Header (Vec2d (0, 0), width, w_name, this);
    canvas_ = new plug::Canvas (width, height - HEADER_HEIGHT, Color (255, 255, 255, 255), Vec2d (0, HEADER_HEIGHT), palette);
    assert (header_ && canvas_ && "failed to allocate window canvas and header\n");

    if (need_scrollbar)
    {
        scrollbar_ = new plug::Scrollbar (Vec2d (width, HEADER_HEIGHT), height - HEADER_HEIGHT, canvas_->height_, height - HEADER_HEIGHT, this);
        assert (scrollbar_ && "failed to allocate window scrollbar \n");

        plug::Widget *scrollbar = scrollbar_;
        contained_widgets.add (scrollbar);
    }

    plug::Widget *header = header_;
    plug::Widget *canvas = canvas_;
    contained_widgets.add (header);
    contained_widgets.add (canvas);
}

plug::Window::~Window ()
{
    width_  = __INT_MAX__; 
    height_ = __INT_MAX__;

    if (header_)    delete header_;
    if (canvas_)    delete canvas_;
    if (scrollbar_) delete scrollbar_;
    delete layout_;
}

void plug::Window::render (sf::RenderTarget &target, plug::TransformStack &transform_stack)
{
    transform_stack.enter (plug::Transform (layout_->get_position ()));

    plug::Vec2d lh_pos = transform_stack.top ().getOffset ();

    sf::RectangleShape rect (plug::Vec2d (width_, height_));
    rect.setFillColor (plug::Color (0, 0,0,0));
    rect.setOutlineColor (plug::Color (50, 50, 50));
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



void plug::Window::onMousePressed     (const plug::MousePressedEvent &event, plug::EHC &ehc)
{
    ehc.stack.enter (plug::Transform (layout_->get_position ()));

    size_t widgets_num = contained_widgets.get_size ();

    for (size_t window_widget_idx = 0; window_widget_idx < widgets_num; ++window_widget_idx)
    {
        if (contained_widgets[window_widget_idx])
        {
            contained_widgets[window_widget_idx]->onEvent (event, ehc);
            if (ehc.stopped == true)
            {
                ehc.stack.leave ();
                return;
            }
        }
    }

    ehc.stack.leave ();
}

void plug::Window::onMouseReleased    (const plug::MouseReleasedEvent &event, plug::EHC &ehc)
{
    ehc.stack.enter (plug::Transform (layout_->get_position ()));

    size_t widgets_num = contained_widgets.get_size ();
    for (size_t window_widget_idx = 0; window_widget_idx < widgets_num; ++window_widget_idx)
    {
        if (contained_widgets[window_widget_idx]) 
            contained_widgets[window_widget_idx]->onEvent (event, ehc);
    }

    ehc.stack.leave ();
}

void plug::Window::onMouseMove        (const plug::MouseMoveEvent &event, plug::EHC &ehc)
{
    ehc.stack.enter (plug::Transform (layout_->get_position ()));

    size_t widgets_num = contained_widgets.get_size ();
    for (size_t window_widget_idx = 0; window_widget_idx < widgets_num; ++window_widget_idx)
    {
        if (contained_widgets[window_widget_idx]) 
        {
            contained_widgets[window_widget_idx]->onEvent (event, ehc);
            if (ehc.stopped == true)
            {
                ehc.stack.leave ();
                return;
            }
        }
    }

    ehc.stack.leave ();
}

void plug::Window::onKeyboardPressed  (const plug::KeyboardPressedEvent &event, plug::EHC &ehc)
{
    size_t widgets_num = contained_widgets.get_size ();

    for (size_t window_widget_idx = 0; window_widget_idx < widgets_num; ++window_widget_idx)
    {
        if (contained_widgets[window_widget_idx])
        {
            contained_widgets[window_widget_idx]->onEvent (event, ehc);
            if (ehc.stopped)
                return;
        }
    }
} 

void plug::Window::onKeyboardReleased (const plug::KeyboardReleasedEvent &event, plug::EHC &ehc)
{
    size_t widgets_num = contained_widgets.get_size ();
    
    for (size_t window_widget_idx = 0; window_widget_idx < widgets_num; ++window_widget_idx)
    {
        if (contained_widgets[window_widget_idx])
            contained_widgets[window_widget_idx]->onEvent (event, ehc);
    }
}

void plug::Window::onTick             (const plug::TickEvent &event, plug::EHC &ehc)
{
    size_t widgets_num = contained_widgets.get_size ();
    for (size_t window_widget_idx = 0; window_widget_idx < widgets_num; ++window_widget_idx)
    {
        if (contained_widgets[window_widget_idx])
        {
            contained_widgets[window_widget_idx]->onEvent (event, ehc);
        }
    }
}

bool plug::Window::contains (const plug::Vec2d &pos)
{   
    plug::Vec2d pos_ = pos - layout_->get_position ();

    double x = pos_.get_x ();
    double y = pos_.get_y ();

    if ((x >= 0 && y >= 0) && 
        (x <= width_ && y <= height_))
        return true;

    return false;  
} 