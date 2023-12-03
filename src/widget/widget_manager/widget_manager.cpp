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
    if (event)
    {
        sf::Event cur_event = *event;
        // printf ("event\n");
        switch (cur_event.type)
        {
            case sf::Event::KeyPressed:
            {
                // printf ("KeyPressed\n");

                plug::KeyboardPressedEvent key_event ((plug::KeyCode)cur_event.key.code, false, false, false);
                // key_event.key_id = (plug::KeyCode)cur_event.key.code;
                
                onKeyboardPressed (key_event, ehc);
                break;
            }
            case sf::Event::KeyReleased:
            {
                // printf ("KeyReleased\n");
                plug::KeyboardReleasedEvent key_event ((plug::KeyCode)cur_event.key.code, false, false, false);
                // key_event.key_id = (plug::KeyCode)cur_event.key.code;
                
                onKeyboardReleased (key_event, ehc);
                break;
            }
            case sf::Event::MouseButtonPressed:
            {
                // printf ("MouseButtonPressed\n");
                plug::Vec2d pos (cur_event.mouseButton.x, cur_event.mouseButton.y);
                
                plug::MousePressedEvent mouse_event ((plug::MouseButton)event->mouseButton.button, pos, false, false, false);
                // mouse_event.pos = pos;
                
                onMousePressed (mouse_event, ehc);

                break;
            }
            case sf::Event::MouseButtonReleased:
            {
                // printf ("MouseButtonReleased\n");
                plug::Vec2d pos (cur_event.mouseButton.x, cur_event.mouseButton.y);
                
                plug::MouseReleasedEvent mouse_event ((plug::MouseButton)event->mouseButton.button, pos, false, false, false);
                // mouse_event.pos = pos;
                
                onMouseReleased (mouse_event, ehc);
                break;
            }
            case sf::Event::MouseMoved:
            {
                // printf ("MouseMoved\n");
                plug::Vec2d pos (cur_event.mouseMove.x, cur_event.mouseMove.y);

                plug::MouseMoveEvent mouse_event (pos, false, false, false);
                // mouse_event.pos = pos;
                
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
    // tick_event.delta_time = delta_time;
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
