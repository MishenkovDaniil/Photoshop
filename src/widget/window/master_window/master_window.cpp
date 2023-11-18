#include "master_window.h"

Master_window::Master_window (int width, int height, Vec2d lh_pos, const char *w_name, int list_capacity) :
    Window (width, height, lh_pos, w_name, false)
{   
    menu_ = new Menu(Vec2d (0, HEADER_HEIGHT), width);
    assert (menu_);
    list_ctor (&windows, 10);
}

Master_window::~Master_window ()
{
    delete menu_;
    list_dtor (&windows);
}

void Master_window::add_menu_button (Button *button)
{
    menu_->add_button (button);
}

void Master_window::add_window (Window *window)
{
    assert (window && "incorrect window pointer in add_window()");

    last_ = list_insert (&windows, last_, window);
}

void Master_window::render (sf::RenderTarget &target, TransformStack &transform_stack) 
{
    Window::render (target, transform_stack);
    
    transform_stack.enter (Transform (layout_->get_position ()));
    menu_->render (target, transform_stack);
    transform_stack.leave ();

    size_t window_list_pos = first_;

    for (int window_idx = 0; window_idx < windows.size; ++window_idx)
    {
        Window *window = (Window *)list_get (&windows, window_list_pos);
        window_list_pos = windows.elems[window_list_pos].next;

        if (!window)
        {
            fprintf (stderr, "Event error: nil button is detected.\n"
                             "Hint: nil button idx in windows list = %lu.\n"
                             "Hint: windows list size = %d.\n", window_list_pos, windows.size);
            return;
        }
        assert (window);

        window->render (target, transform_stack);
    }
}

void Master_window::onMousePressed     (MousePressedEvent &event, EHC &ehc)
{
    size_t window_list_pos = last_;

    for (int window_idx = 0; window_idx < windows.size; ++window_idx)
    {
        Window *window = (Window *)list_get (&windows, window_list_pos);

        if (!window)
        {
            fprintf (stderr, "Event error: nil button is detected.\n"
                             "Hint: nil button idx in windows list = %lu.\n"
                             "Hint: windows list size = %d.\n", window_list_pos, windows.size);
            ehc.stopped = true;
            return;
        }
        assert (window);

        if (window->contains (event.pos))
        {
            window->onMousePressed (event, ehc);
            if (ehc.stopped)
            {
                // active_window = window_list_pos;
                if (last_ != window_list_pos)
                {
                    if (first_ == window_list_pos) 
                    {
                        first_ = windows.elems[window_list_pos].next;
                    }
                    list_remove (&windows, window_list_pos);
                    last_ = list_insert (&windows, last_, window);
                }
                return;
            }
        }
        window_list_pos = windows.elems[window_list_pos].prev;
    }
    
    Window::onMousePressed (event, ehc);
    if (ehc.stopped)
        return;

    ehc.stack.enter (Transform (layout_->get_position ()));
    
    menu_->onMousePressed (event, ehc);
    printf ("ehc.stopped = %d\n", ehc.stopped);
    ehc.stack.leave ();
}   

void Master_window::onMouseReleased    (MouseReleasedEvent &event, EHC &ehc)
{
    size_t window_list_pos = last_;

    for (int window_idx = 0; window_idx < windows.size; ++window_idx)
    {
        Window *window = (Window *)list_get (&windows, window_list_pos);
        if (!window)
        {
            fprintf (stderr, "Event error: nil button is detected.\n"
                             "Hint: nil button idx in windows list = %lu.\n"
                             "Hint: windows list size = %d.\n", window_list_pos, windows.size);
            ehc.stopped = true;
            return;
        }
        assert (window);

        window->onMouseReleased (event, ehc);
        
        window_list_pos = windows.elems[window_list_pos].prev;
    }

    Window::onMouseReleased (event, ehc);

    ehc.stack.enter (Transform (layout_->get_position ()));
    menu_->onMouseReleased (event, ehc);
    ehc.stack.leave ();
}

void Master_window::onMouseMove        (MouseMoveEvent &event, EHC &ehc)
{
    Transform tr (layout_->get_position ());
    Vec2d pos_ = tr.apply (event.pos);

    size_t window_list_pos = last_;

    for (int window_idx = 0; window_idx < windows.size; ++window_idx)
    {
        Window *window = (Window *)list_get (&windows, window_list_pos);
        if (!window)
        {
            fprintf (stderr, "Event error: nil button is detected.\n"
                             "Hint: nil button idx in windows list = %lu.\n"
                             "Hint: windows list size = %d.\n", window_list_pos, windows.size);
            ehc.stopped = true;
            return;
        }
        assert (window);

        window->onMouseMove (event, ehc);
        if (ehc.stopped)
        {
            return;
        }

        window_list_pos = windows.elems[window_list_pos].prev;
    }

    Window::onMouseMove (event, ehc);
    if (ehc.stopped)
        return;

    event.pos = pos_; // should do pos_ through transformStack
    menu_->onMouseMove (event, ehc);
}

void Master_window::onKeyboardPressed  (KeyboardPressedEvent &event, EHC &ehc)
{
    bool status = false;

    size_t window_list_pos = last_;

    for (int window_idx = 0; window_idx < windows.size; ++window_idx)
    {
        Window *window = (Window *)list_get (&windows, window_list_pos);
        if (!window)
        {
            fprintf (stderr, "Event error: nil button is detected.\n"
                             "Hint: nil button idx in windows list = %lu.\n"
                             "Hint: windows list size = %d.\n", window_list_pos, windows.size);
            ehc.stopped = true;
            return;
        }
        assert (window);

        window->onKeyboardPressed (event, ehc);
        if (ehc.stopped)
            return;
        window_list_pos = windows.elems[window_list_pos].prev;
    }

    Window::onKeyboardPressed (event, ehc);
    if (ehc.stopped)
        return;

    menu_->onKeyboardPressed (event, ehc);
}

void Master_window::onKeyboardReleased (KeyboardReleasedEvent &event, EHC &ehc)
{
    size_t window_list_pos = last_;
    for (int window_idx = 0; window_idx < windows.size; ++window_idx)
    {
        Window *window = (Window *)list_get (&windows, window_list_pos);
        if (!window)
        {
            fprintf (stderr, "Event error: nil button is detected.\n"
                             "Hint: nil button idx in windows list = %lu.\n"
                             "Hint: windows list size = %d.\n", window_list_pos, windows.size);
            ehc.stopped = true;
            return;
        }
        assert (window);

        window->onKeyboardReleased (event, ehc);
        window_list_pos = windows.elems[window_list_pos].prev;
    }

    Window::onKeyboardReleased (event, ehc);
    menu_->onKeyboardReleased (event, ehc);
}

void Master_window::onTick             (TickEvent &event, EHC &ehc)
{
    size_t window_list_pos = last_;
    for (int window_idx = 0; window_idx < windows.size; ++window_idx)
    {
        Window *window = (Window *)list_get (&windows, window_list_pos);
        if (!window)
        {
            fprintf (stderr, "Event error: nil button is detected.\n"
                             "Hint: nil button idx in windows list = %lu.\n"
                             "Hint: windows list size = %d.\n", window_list_pos, windows.size);
            ehc.stopped = true;
            return;
        }
        assert (window);

        window->onTick (event, ehc);
        window_list_pos = windows.elems[window_list_pos].prev;
    }

    Window::onTick (event, ehc);
    menu_->onTick (event, ehc);
}





// bool Master_window::on_mouse_pressed  (MouseButton mouse_button, Vec2d&pos, TransformStack &transform_stack) 
// {
//     size_t window_list_pos = last_;

//     for (int window_idx = 0; window_idx < windows.size; ++window_idx)
//     {
//         Window *window = (Window *)list_get (&windows, window_list_pos);

//         if (!window)
//         {
//             fprintf (stderr, "Event error: nil button is detected.\n"
//                              "Hint: nil button idx in windows list = %lu.\n"
//                              "Hint: windows list size = %d.\n", window_list_pos, windows.size);
//             return false;
//         }
//         assert (window);

//         if (window->contains (pos)) //to pos_
//         {
//             bool is_released_on_child = window->on_mouse_pressed (mouse_button, pos, transform_stack);
//             if (is_released_on_child)
//             {
//                 // active_window = window_list_pos;
//                 if (last_ != window_list_pos)
//                 {
//                     if (first_ == window_list_pos) 
//                     {
//                         first_ = windows.elems[window_list_pos].next;
//                     }
//                     list_remove (&windows, window_list_pos);
//                     last_ = list_insert (&windows, last_, window);
//                 }
//                 return true;
//             }
//         }
//         window_list_pos = windows.elems[window_list_pos].prev;
//     }
    
//     if (Window::on_mouse_pressed (mouse_button, pos, transform_stack))
//         return true;

//     transform_stack.enter (Transform (layout_->get_position ()));
    
//     if (menu_->on_mouse_pressed (mouse_button, pos, transform_stack))
//     {
//         transform_stack.leave ();
//         return true;
//     }
//     transform_stack.leave ();

//     return false;   
// }

// bool Master_window::on_mouse_released (MouseButton mouse_button, Vec2d&pos, TransformStack &transform_stack) 
// {
//     bool status = false;

//     size_t window_list_pos = last_;

//     for (int window_idx = 0; window_idx < windows.size; ++window_idx)
//     {
//         Window *window = (Window *)list_get (&windows, window_list_pos);
//         if (!window)
//         {
//             fprintf (stderr, "Event error: nil button is detected.\n"
//                              "Hint: nil button idx in windows list = %lu.\n"
//                              "Hint: windows list size = %d.\n", window_list_pos, windows.size);
//             return false;
//         }
//         assert (window);

//         status |= window->on_mouse_released (mouse_button, pos, transform_stack);
        
//         window_list_pos = windows.elems[window_list_pos].prev;
//     }

//     status |= Window::on_mouse_released (mouse_button, pos, transform_stack);

//     transform_stack.enter (Transform (layout_->get_position ()));
//     status |= (menu_->on_mouse_released (mouse_button, pos, transform_stack));
//     transform_stack.leave ();

//     return status;   
// }

// bool Master_window::on_mouse_moved (Vec2d&new_pos, TransformStack &transform_stack) 
// {
//     Transform tr (layout_->get_position ());
//     Vec2d pos_ = tr.apply (new_pos);

//     size_t window_list_pos = last_;

//     for (int window_idx = 0; window_idx < windows.size; ++window_idx)
//     {
//         Window *window = (Window *)list_get (&windows, window_list_pos);
//         if (!window)
//         {
//             fprintf (stderr, "Event error: nil button is detected.\n"
//                              "Hint: nil button idx in windows list = %lu.\n"
//                              "Hint: windows list size = %d.\n", window_list_pos, windows.size);
//             return false;
//         }
//         assert (window);

//         bool is_released_on_child = window->on_mouse_moved (new_pos, transform_stack);
//         if (is_released_on_child)
//         {
//             return true;
//         }

//         window_list_pos = windows.elems[window_list_pos].prev;
//     }

//     if (Window::on_mouse_moved (new_pos, transform_stack))
//         return true;

//     if (menu_->on_mouse_moved (pos_, transform_stack))
//     {
//         return true;
//     }

//     return false;
// }   

// bool Master_window::on_keyboard_pressed  (KeyCode key) 
// {
//     bool status = false;

//     size_t window_list_pos = last_;

//     for (int window_idx = 0; window_idx < windows.size; ++window_idx)
//     {
//         Window *window = (Window *)list_get (&windows, window_list_pos);
//         if (!window)
//         {
//             fprintf (stderr, "Event error: nil button is detected.\n"
//                              "Hint: nil button idx in windows list = %lu.\n"
//                              "Hint: windows list size = %d.\n", window_list_pos, windows.size);
//             return false;
//         }
//         assert (window);

//         status |= window->on_keyboard_pressed (key);
//         if (status)
//             return true;
//         window_list_pos = windows.elems[window_list_pos].prev;
//     }

//     status |= Window::on_keyboard_pressed (key);
//     if (status)
//         return true;
//     status |= menu_->on_keyboard_pressed (key);

//     return status;   
// }

// bool Master_window::on_keyboard_released (KeyCode key) 
// {
//     bool status = false;

//     size_t window_list_pos = last_;

//     for (int window_idx = 0; window_idx < windows.size; ++window_idx)
//     {
//         Window *window = (Window *)list_get (&windows, window_list_pos);
//         if (!window)
//         {
//             fprintf (stderr, "Event error: nil button is detected.\n"
//                              "Hint: nil button idx in windows list = %lu.\n"
//                              "Hint: windows list size = %d.\n", window_list_pos, windows.size);
//             return false;
//         }
//         assert (window);

//         status |= window->on_keyboard_released (key);
//         window_list_pos = windows.elems[window_list_pos].prev;
//     }

//     status |= Window::on_keyboard_released (key);
//     status |= menu_->on_keyboard_released (key);

//     return status;   
// }

// bool Master_window::on_tick (float delta_sec)
// {
//     bool status = false;
    
//     size_t window_list_pos = last_;

//     for (int window_idx = 0; window_idx < windows.size; ++window_idx)
//     {
//         Window *window = (Window *)list_get (&windows, window_list_pos);
//         if (!window)
//         {
//             fprintf (stderr, "Event error: nil button is detected.\n"
//                              "Hint: nil button idx in windows list = %lu.\n"
//                              "Hint: windows list size = %d.\n", window_list_pos, windows.size);
//             return false;
//         }
//         assert (window);

//         status |= window->on_tick (delta_sec);
//         window_list_pos = windows.elems[window_list_pos].prev;
//     }

//     status |= Window::on_tick (delta_sec);
//     status |= menu_->on_tick (delta_sec);
    
//     return status;
// }
