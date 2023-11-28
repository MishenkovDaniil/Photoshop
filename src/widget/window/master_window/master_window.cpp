#include "master_window.h"

Master_window::Master_window (int width, int height, plug::Vec2d lh_pos, const char *w_name, int list_capacity) :
    Window (width, height, lh_pos, w_name, false)
{   
    menu_ = new Menu(plug::Vec2d (0, HEADER_HEIGHT), width);
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

void Master_window::render (plug::RenderTarget &target, plug::TransformStack &transform_stack) 
{
    Window::render (target, transform_stack);
    
    transform_stack.enter (plug::Transform (layout_->getPosition ()));
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

void Master_window::onMousePressed     (const plug::MousePressedEvent &event, plug::EHC &ehc)
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

        if (window->covers (ehc.stack, event.pos))
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

    ehc.stack.enter (plug::Transform (layout_->getPosition ()));
    
    menu_->onMousePressed (event, ehc);
    ehc.stack.leave ();
}   

void Master_window::onMouseReleased    (const plug::MouseReleasedEvent &event, plug::EHC &ehc)
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

    ehc.stack.enter (plug::Transform (layout_->getPosition ()));
    menu_->onMouseReleased (event, ehc);
    ehc.stack.leave ();
}

void Master_window::onMouseMove        (const plug::MouseMoveEvent &event, plug::EHC &ehc)
{
    plug::Transform tr (layout_->getPosition ());
    plug::Vec2d pos_ = tr.apply (event.pos);

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

    // event.pos = pos_; // should do pos_ through transformStack
    ehc.stack.enter (layout_->getPosition ());

    menu_->onMouseMove (event, ehc);

    ehc.stack.leave ();
}

void Master_window::onKeyboardPressed  (const plug::KeyboardPressedEvent &event, plug::EHC &ehc)
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

void Master_window::onKeyboardReleased (const plug::KeyboardReleasedEvent &event, plug::EHC &ehc)
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

void Master_window::onTick             (const plug::TickEvent &event, plug::EHC &ehc)
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
