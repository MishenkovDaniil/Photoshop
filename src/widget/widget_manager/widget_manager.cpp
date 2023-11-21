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

void Widget_manager::add_widget (Widget *widget)
{
    assert (widget && "added widget is nullptr");
    
    list_insert (&widgets, 0, widget);
}

void Widget_manager::render (sf::RenderTarget &target)
{
    for (int widget_idx = 0; widget_idx < widgets.size; ++widget_idx)
    {
        Widget *widget = (Widget *)list_get (&widgets, widget_idx + 1);
        if (!widget)
        {
            fprintf (stderr, "Event error: nil widget is detected.\n"
                             "Hint: nil widget idx in widgets list = %d.\n"
                             "Hint: widgets list size = %d.\n", widget_idx + 1, widgets.size);
            return;
        }
        assert (widget);

        widget->render (target, transform_stack_);
    }
}

void Widget_manager::onEvent (sf::Event *event, double delta_time)
{
    EHC ehc (transform_stack_);
    if (event)
    {
        sf::Event cur_event = *event;
        // printf ("event\n");
        switch (cur_event.type)
        {
            case sf::Event::KeyPressed:
            {
                // printf ("KeyPressed\n");

                KeyboardPressedEvent key_event;
                key_event.key_id = (KeyCode)cur_event.key.code;
                
                onKeyboardPressed (key_event, ehc);
                break;
            }
            case sf::Event::KeyReleased:
            {
                // printf ("KeyReleased\n");
                KeyboardReleasedEvent key_event;
                key_event.key_id = (KeyCode)cur_event.key.code;
                
                onKeyboardReleased (key_event, ehc);
                break;
            }
            case sf::Event::MouseButtonPressed:
            {
                // printf ("MouseButtonPressed\n");
                Vec2d pos (cur_event.mouseButton.x, cur_event.mouseButton.y);
                
                MousePressedEvent mouse_event;
                mouse_event.pos = pos;
                
                onMousePressed (mouse_event, ehc);

                break;
            }
            case sf::Event::MouseButtonReleased:
            {
                // printf ("MouseButtonReleased\n");
                Vec2d pos (cur_event.mouseButton.x, cur_event.mouseButton.y);
                
                MouseReleasedEvent mouse_event;
                mouse_event.pos = pos;
                
                onMouseReleased (mouse_event, ehc);
                break;
            }
            case sf::Event::MouseMoved:
            {
                // printf ("MouseMoved\n");
                Vec2d pos (cur_event.mouseMove.x, cur_event.mouseMove.y);

                MouseMoveEvent mouse_event;
                mouse_event.pos = pos;
                
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
    TickEvent tick_event;
    tick_event.delta_time = delta_time;
    ehc.overlapped = ehc.stopped = false;
    onTick (tick_event, ehc);
}

void Widget_manager::onMousePressed     (const MousePressedEvent &event, EHC &ehc)
{
    // bool is_pressed_on_child = false;
    for (int widget_idx = 0; widget_idx < widgets.size; ++widget_idx)
    {
        Widget *widget = (Widget *)list_get (&widgets, widget_idx + 1);
        if (!widget)
        {
            fprintf (stderr, "Event error: nil widget is detected.\n"
                             "Hint: nil widget idx in widgets list = %d.\n"
                             "Hint: widgets list size = %d.\n", widget_idx + 1, widgets.size);
            ehc.stopped = true;
            return;
        }
        assert (widget);

        widget->onMousePressed (event, ehc);
        if (ehc.stopped == true)
            return;
    }

    // return false;
}

void Widget_manager::onMouseReleased    (const MouseReleasedEvent &event, EHC &ehc)
{
    for (int widget_idx = 0; widget_idx < widgets.size; ++widget_idx)
    {
        Widget *widget = (Widget *)list_get (&widgets, widget_idx + 1);
        if (!widget)
        {
            fprintf (stderr, "Event error: nil widget is detected.\n"
                             "Hint: nil widget idx in widgets list = %d.\n"
                             "Hint: widgets list size = %d.\n", widget_idx + 1, widgets.size);
            ehc.stopped = true;
            return;
        }
        assert (widget);

        widget->onMouseReleased (event, ehc);
    }
}

void Widget_manager::onMouseMove        (const MouseMoveEvent &event, EHC &ehc)
{
    bool is_moved = false;
    for (int widget_idx = 0; widget_idx < widgets.size; ++widget_idx)
    {
        Widget *widget = (Widget *)list_get (&widgets, widget_idx + 1);
        if (!widget)
        {
            fprintf (stderr, "Event error: nil widget is detected.\n"
                             "Hint: nil widget idx in widgets list = %d.\n"
                             "Hint: widgets list size = %d.\n", widget_idx + 1, widgets.size);
            ehc.stopped = true;
            return;
        }
        assert (widget);

        widget->onMouseMove (event, ehc);
        if (ehc.stopped)
            return;
    }

    // return false;
}

void Widget_manager::onKeyboardPressed  (const KeyboardPressedEvent &event, EHC &ehc)
{
    bool is_keyboard_pressed = false;
    for (int widget_idx = 0; widget_idx < widgets.size; ++widget_idx)
    {
        Widget *widget = (Widget *)list_get (&widgets, widget_idx + 1);
        if (!widget)
        {
            fprintf (stderr, "Event error: nil widget is detected.\n"
                             "Hint: nil widget idx in widgets list = %d.\n"
                             "Hint: widgets list size = %d.\n", widget_idx + 1, widgets.size);
            ehc.stopped = true;
            return;
        }
        assert (widget);

        widget->onKeyboardPressed (event, ehc);
        if (ehc.stopped)
            return;
    }

    // return false;
}

void Widget_manager::onKeyboardReleased (const KeyboardReleasedEvent &event, EHC &ehc)
{
    for (int widget_idx = 0; widget_idx < widgets.size; ++widget_idx)
    {
        Widget *widget = (Widget *)list_get (&widgets, widget_idx + 1);
        if (!widget)
        {
            fprintf (stderr, "Event error: nil widget is detected.\n"
                             "Hint: nil widget idx in widgets list = %d.\n"
                             "Hint: widgets list size = %d.\n", widget_idx + 1, widgets.size);
            ehc.stopped = true;
            return;
        }
        assert (widget);

        widget->onKeyboardReleased (event, ehc);
        if (ehc.stopped)
            return;
    }

    // return false;
}

void Widget_manager::onTick             (const TickEvent &event, EHC &ehc)
{
    // bool on_time_status = false;
    
    for (int widget_idx = 0; widget_idx < widgets.size; ++widget_idx)
    {
        Widget *widget = (Widget *)list_get (&widgets, widget_idx + 1);
        if (!widget)
        {
            fprintf (stderr, "Event error: nil widget is detected.\n"
                             "Hint: nil widget idx in widgets list = %d.\n"
                             "Hint: widgets list size = %d.\n", widget_idx + 1, widgets.size);
            ehc.stopped = true;
            return;
        }
        assert (widget);

        widget->onTick (event, ehc);
    }

    // return on_time_status;
}
