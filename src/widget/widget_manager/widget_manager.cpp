#include <cassert>
#include <iostream>

#include "widget_manager.h"

Widget_manager::Widget_manager (int list_capacity) :
    transform_stack_ (M_vector (Transform (Vector (0, 0))))
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

bool Widget_manager::on_mouse_pressed  (Mouse_key mouse_key, Vector &pos)
{
    bool is_pressed_on_child = false;
    for (int widget_idx = 0; widget_idx < widgets.size; ++widget_idx)
    {
        Widget *widget = (Widget *)list_get (&widgets, widget_idx + 1);
        if (!widget)
        {
            fprintf (stderr, "Event error: nil widget is detected.\n"
                             "Hint: nil widget idx in widgets list = %d.\n"
                             "Hint: widgets list size = %d.\n", widget_idx + 1, widgets.size);
            return false;
        }
        assert (widget);

        is_pressed_on_child = widget->on_mouse_pressed (mouse_key, pos, transform_stack_);
        
        if (is_pressed_on_child)
            return true;
    }

    return false;
}

bool Widget_manager::on_mouse_released (Mouse_key mouse_key, Vector &pos)
{
    bool is_released_on_child = false;
    for (int widget_idx = 0; widget_idx < widgets.size; ++widget_idx)
    {
        Widget *widget = (Widget *)list_get (&widgets, widget_idx + 1);
        if (!widget)
        {
            fprintf (stderr, "Event error: nil widget is detected.\n"
                             "Hint: nil widget idx in widgets list = %d.\n"
                             "Hint: widgets list size = %d.\n", widget_idx + 1, widgets.size);
            return false;
        }
        assert (widget);

        is_released_on_child = widget->on_mouse_released (mouse_key, pos, transform_stack_);
        if (is_released_on_child)
            return true;
    }

    return false;
}

bool Widget_manager::on_mouse_moved (Vector &new_pos)
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
            return false;
        }
        assert (widget);

        is_moved = widget->on_mouse_moved (new_pos, transform_stack_);
        if (is_moved)
            return true;
    }

    return false;
}   

bool Widget_manager::on_keyboard_pressed  (Keyboard_key key)
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
            return false;
        }
        assert (widget);

        is_keyboard_pressed = widget->on_keyboard_pressed (key);
        if (is_keyboard_pressed)
            return true;
    }

    return false;
}

bool Widget_manager::on_keyboard_released (Keyboard_key key)
{
    bool is_keyboard_released = false;
    for (int widget_idx = 0; widget_idx < widgets.size; ++widget_idx)
    {
        Widget *widget = (Widget *)list_get (&widgets, widget_idx + 1);
        if (!widget)
        {
            fprintf (stderr, "Event error: nil widget is detected.\n"
                             "Hint: nil widget idx in widgets list = %d.\n"
                             "Hint: widgets list size = %d.\n", widget_idx + 1, widgets.size);
            return false;
        }
        assert (widget);

        is_keyboard_released = widget->on_keyboard_released (key);
        if (is_keyboard_released)
            return true;
    }

    return false;
}

bool Widget_manager::on_time (float delta_sec)
{
    bool on_time_status = false;
    
    for (int widget_idx = 0; widget_idx < widgets.size; ++widget_idx)
    {
        Widget *widget = (Widget *)list_get (&widgets, widget_idx + 1);
        if (!widget)
        {
            fprintf (stderr, "Event error: nil widget is detected.\n"
                             "Hint: nil widget idx in widgets list = %d.\n"
                             "Hint: widgets list size = %d.\n", widget_idx + 1, widgets.size);
            return false;
        }
        assert (widget);

        on_time_status |= widget->on_time (delta_sec);
    }

    return on_time_status;
}