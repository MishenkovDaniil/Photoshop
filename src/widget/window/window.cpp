#include <iostream>
#include <cassert>

#include "window.h"

#include "../widget.h"
#include "menu/menu.h"
#include "header/header.h"
#include "canvas/canvas.h"
#include "scrollbar/scrollbar.h"
#include "master_window/master_window.h"

Window::Window (int width, int height, plug::Vec2d lh_pos, const char *w_name, bool need_scrollbar, Tool_palette *palette) :
    width_ (width),
    height_ (height),
    contained_widgets (nullptr, 3)
{
    layout_= new Default_layout_box (lh_pos, plug::Vec2d (width, height));

    header_ = new Header (plug::Vec2d (0, 0), width, w_name, this);
    canvas_ = new CanvasView (width, height - HEADER_HEIGHT, plug::White, plug::Vec2d (0, HEADER_HEIGHT), palette);
    assert (header_ && canvas_ && "failed to allocate window canvas and header\n");

    if (need_scrollbar)
    {
        scrollbar_ = new Scrollbar (plug::Vec2d (width, HEADER_HEIGHT), height - HEADER_HEIGHT, canvas_->getViewSize ().y, height - HEADER_HEIGHT, this);
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

    if (parent_window_)
    {
        parent_window_->rm_window (this);
    }
}

void Window::setCanvasImage (plug::Texture &new_texture)
{
    plug::VertexArray vertices (plug::Quads, 4);
    const plug::Texture &texture = canvas_->getCanvas ()->canvas_img;

    
    vertices[0].position = plug::Vec2d (0, 0);
    vertices[1].position = plug::Vec2d (0, texture.height);
    vertices[2].position = plug::Vec2d (texture.width, texture.height);
    vertices[3].position = plug::Vec2d (texture.width, 0);

    vertices[0].color = plug::White;
    vertices[1].color = plug::White;
    vertices[2].color = plug::White;
    vertices[3].color = plug::White;
    
    vertices[0].tex_coords = plug::Vec2d (0, 0);
    vertices[1].tex_coords = plug::Vec2d (0, new_texture.height - 1);
    vertices[2].tex_coords = plug::Vec2d (new_texture.width - 1, new_texture.height - 1);
    vertices[3].tex_coords = plug::Vec2d (new_texture.width - 1, 0);
    canvas_->getCanvas ()->draw (vertices, new_texture);
}


void Window::draw (plug::TransformStack &transform_stack, plug::RenderTarget &target)
{
    transform_stack.enter (plug::Transform (layout_->getPosition ()));

    plug::Vec2d lh_pos = transform_stack.top ().getOffset ();

    RectangleShape rect (plug::Vec2d (width_, height_));
    rect.setFillColor (plug::Color (0, 0, 0, 0));
    rect.setOutlineColor (plug::Color (50, 50, 50));
    rect.setOutlineThickness (-1);
    rect.setPosition (lh_pos);

    canvas_->draw (transform_stack, target);

    if (scrollbar_) 
    {
        scrollbar_->draw (transform_stack, target);
    }
    
    header_->draw (transform_stack, target);
    
    ((RenderTexture &)target).draw (rect);

    transform_stack.leave ();
}


void Window::onMousePressed     (const plug::MousePressedEvent &event, plug::EHC &ehc)
{
    ehc.stack.enter (plug::Transform (layout_->getPosition ()));

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

void Window::onMouseReleased    (const plug::MouseReleasedEvent &event, plug::EHC &ehc)
{
    ehc.stack.enter (plug::Transform (layout_->getPosition ()));

    size_t widgets_num = contained_widgets.get_size ();
    for (size_t window_widget_idx = 0; window_widget_idx < widgets_num; ++window_widget_idx)
    {
        if (contained_widgets[window_widget_idx]) 
            contained_widgets[window_widget_idx]->onEvent (event, ehc);
    }

    ehc.stack.leave ();
}

void Window::onMouseMove        (const plug::MouseMoveEvent &event, plug::EHC &ehc)
{
    ehc.stack.enter (plug::Transform (layout_->getPosition ()));

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

void Window::onKeyboardPressed  (const plug::KeyboardPressedEvent &event, plug::EHC &ehc)
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

void Window::onKeyboardReleased (const plug::KeyboardReleasedEvent &event, plug::EHC &ehc)
{
    size_t widgets_num = contained_widgets.get_size ();
    
    for (size_t window_widget_idx = 0; window_widget_idx < widgets_num; ++window_widget_idx)
    {
        if (contained_widgets[window_widget_idx])
            contained_widgets[window_widget_idx]->onEvent (event, ehc);
    }
}

void Window::onTick             (const plug::TickEvent &event, plug::EHC &ehc)
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