#include <cassert>
#include <iostream>

#include "widget_manager.h"

Widget_manager::Widget_manager (int list_capacity)
{
    list_ctor (&widgets, list_capacity);
}

Widget_manager::~Widget_manager ()
{
    list_dtor (&widgets);
}

void Widget_manager::add_widget (plug::Widget *widget)
{
    assert (widget && "added widget is nullptr");
    
    list_insert (&widgets, 0, widget);
}

void Widget_manager::draw (plug::RenderTarget &target)
{
    for (int widget_idx = 0; widget_idx < widgets.size; ++widget_idx)
    {
        plug::Widget *widget = (plug::Widget *)list_get (&widgets, widget_idx + 1);
        if (!widget)
        {
            fprintf (stderr, "Event error: nil widget is detected.\n"
                             "Hint: nil widget idx in widgets list = %d.\n"
                             "Hint: widgets list size = %d.\n", widget_idx + 1, widgets.size);
            return;
        }
        assert (widget);

        widget->draw (transform_stack_, target);
    }
}

void Widget_manager::onEvent (sf::Event *event, double delta_time)
{
    plug::EHC ehc = {transform_stack_, false, false};
    
    static bool shift = false;
    static bool control = false;
    static bool alt = false;

    if (event)
    {
        sf::Event cur_event = *event;

        switch (cur_event.type)
        {
            case sf::Event::KeyPressed:
            {
                plug::KeyboardPressedEvent key_event ((plug::KeyCode)cur_event.key.code, cur_event.key.shift, cur_event.key.control, cur_event.key.alt);

                shift = cur_event.key.shift;
                control = cur_event.key.control;
                alt = cur_event.key.alt;

                onKeyboardPressed (key_event, ehc);
                break;
            }
            case sf::Event::KeyReleased:
            {
                plug::KeyboardReleasedEvent key_event ((plug::KeyCode)cur_event.key.code, cur_event.key.shift, cur_event.key.control, cur_event.key.alt);

                shift = cur_event.key.shift;
                control = cur_event.key.control;
                alt = cur_event.key.alt;

                onKeyboardReleased (key_event, ehc);
                break;
            }
            case sf::Event::MouseButtonPressed:
            {
                plug::Vec2d pos (cur_event.mouseButton.x, cur_event.mouseButton.y);
                
                plug::MousePressedEvent mouse_event ((plug::MouseButton)event->mouseButton.button, pos, shift, control, alt);
                
                onMousePressed (mouse_event, ehc);

                break;
            }
            case sf::Event::MouseButtonReleased:
            {
                plug::Vec2d pos (cur_event.mouseButton.x, cur_event.mouseButton.y);
                
                plug::MouseReleasedEvent mouse_event ((plug::MouseButton)event->mouseButton.button, pos, shift, control, alt);
                
                onMouseReleased (mouse_event, ehc);
                break;
            }
            case sf::Event::MouseMoved:
            {
                plug::Vec2d pos (cur_event.mouseMove.x, cur_event.mouseMove.y);

                plug::MouseMoveEvent mouse_event (pos, shift, control, alt);
                
                onMouseMove (mouse_event, ehc);
                break;
            }
            default:
            {
                // printf ("default\n");
                break;
            }
        }
    }
    plug::TickEvent tick_event  (delta_time);
    ehc.overlapped = ehc.stopped = false;
    onTick (tick_event, ehc);
}

void Widget_manager::onMousePressed     (const plug::MousePressedEvent &event, plug::EHC &ehc)
{
    // bool is_pressed_on_child = false;
    for (int widget_idx = 0; widget_idx < widgets.size; ++widget_idx)
    {
        plug::Widget *widget = (plug::Widget *)list_get (&widgets, widget_idx + 1);
        if (!widget)
        {
            fprintf (stderr, "Event error: nil widget is detected.\n"
                             "Hint: nil widget idx in widgets list = %d.\n"
                             "Hint: widgets list size = %d.\n", widget_idx + 1, widgets.size);
            ehc.stopped = true;
            return;
        }
        assert (widget);

        widget->onEvent (event, ehc);
        if (ehc.stopped == true)
            return;
    }

    // return false;
}

void Widget_manager::onMouseReleased    (const plug::MouseReleasedEvent &event, plug::EHC &ehc)
{
    for (int widget_idx = 0; widget_idx < widgets.size; ++widget_idx)
    {
        plug::Widget *widget = (plug::Widget *)list_get (&widgets, widget_idx + 1);
        if (!widget)
        {
            fprintf (stderr, "Event error: nil widget is detected.\n"
                             "Hint: nil widget idx in widgets list = %d.\n"
                             "Hint: widgets list size = %d.\n", widget_idx + 1, widgets.size);
            ehc.stopped = true;
            return;
        }
        assert (widget);

        widget->onEvent (event, ehc);
    }
}

void Widget_manager::onMouseMove        (const plug::MouseMoveEvent &event, plug::EHC &ehc)
{
    bool is_moved = false;
    for (int widget_idx = 0; widget_idx < widgets.size; ++widget_idx)
    {
        plug::Widget *widget = (plug::Widget *)list_get (&widgets, widget_idx + 1);
        if (!widget)
        {
            fprintf (stderr, "Event error: nil widget is detected.\n"
                             "Hint: nil widget idx in widgets list = %d.\n"
                             "Hint: widgets list size = %d.\n", widget_idx + 1, widgets.size);
            ehc.stopped = true;
            return;
        }
        assert (widget);

        widget->onEvent (event, ehc);
        if (ehc.stopped)
            return;
    }

    // return false;
}

void Widget_manager::onKeyboardPressed  (const plug::KeyboardPressedEvent &event, plug::EHC &ehc)
{
    bool is_keyboard_pressed = false;
    for (int widget_idx = 0; widget_idx < widgets.size; ++widget_idx)
    {
        plug::Widget *widget = (plug::Widget *)list_get (&widgets, widget_idx + 1);
        if (!widget)
        {
            fprintf (stderr, "Event error: nil widget is detected.\n"
                             "Hint: nil widget idx in widgets list = %d.\n"
                             "Hint: widgets list size = %d.\n", widget_idx + 1, widgets.size);
            ehc.stopped = true;
            return;
        }
        assert (widget);

        widget->onEvent (event, ehc);
        if (ehc.stopped)
            return;
    }

    // return false;
}

void Widget_manager::onKeyboardReleased (const plug::KeyboardReleasedEvent &event, plug::EHC &ehc)
{
    for (int widget_idx = 0; widget_idx < widgets.size; ++widget_idx)
    {
        plug::Widget *widget = (plug::Widget *)list_get (&widgets, widget_idx + 1);
        if (!widget)
        {
            fprintf (stderr, "Event error: nil widget is detected.\n"
                             "Hint: nil widget idx in widgets list = %d.\n"
                             "Hint: widgets list size = %d.\n", widget_idx + 1, widgets.size);
            ehc.stopped = true;
            return;
        }
        assert (widget);

        widget->onEvent (event, ehc);
        if (ehc.stopped)
            return;
    }

    // return false;
}

void Widget_manager::onTick             (const plug::TickEvent &event, plug::EHC &ehc)
{
    // bool on_time_status = false;
    
    for (int widget_idx = 0; widget_idx < widgets.size; ++widget_idx)
    {
        plug::Widget *widget = (plug::Widget *)list_get (&widgets, widget_idx + 1);
        if (!widget)
        {
            fprintf (stderr, "Event error: nil widget is detected.\n"
                             "Hint: nil widget idx in widgets list = %d.\n"
                             "Hint: widgets list size = %d.\n", widget_idx + 1, widgets.size);
            ehc.stopped = true;
            return;
        }
        assert (widget);

        widget->onEvent (event, ehc);
    }

    // return on_time_status;
}
