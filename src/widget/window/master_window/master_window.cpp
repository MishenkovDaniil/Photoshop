#include "master_window.h"

Master_window::Master_window (int width, int height, Vector lh_pos, const char *w_name, int list_capacity) :
    Window (width, height, lh_pos, w_name, false)
{   
    menu_ = new Menu(Vector (0, HEADER_HEIGHT), width);
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

    list_insert (&windows, 0, window);
}

void Master_window::render (sf::RenderTarget &target, M_vector<Transform> &transform_stack) 
{
    Window::render (target, transform_stack);
    
    Transform top = transform_stack.get_size () > 0 ? transform_stack.top () : Transform (Vector (0, 0));
    Transform unite = transform_.unite (top);
    transform_stack.push (unite);

    menu_->render (target, transform_stack);
    transform_stack.pop ();

    for (int window_idx = 0; window_idx < windows.size; ++window_idx)
    {
        Window *window = (Window *)list_get (&windows, window_idx + 1);
        if (!window)
        {
            fprintf (stderr, "Event error: nil button is detected.\n"
                             "Hint: nil button idx in windows list = %d.\n"
                             "Hint: windows list size = %d.\n", window_idx + 1, windows.size);
            return;
        }
        assert (window);

        window->render (target, transform_stack);
    }
}

bool Master_window::on_mouse_pressed  (Mouse_key mouse_key, Vector &pos) 
{
    Vector pos_ = transform_.apply_transform (pos);

    for (int window_idx = 0; window_idx < windows.size; ++window_idx)
    {
        Window *window = (Window *)list_get (&windows, window_idx + 1);
        if (!window)
        {
            fprintf (stderr, "Event error: nil button is detected.\n"
                             "Hint: nil button idx in windows list = %d.\n"
                             "Hint: windows list size = %d.\n", window_idx + 1, windows.size);
            return false;
        }
        assert (window);

        bool is_released_on_child = window->on_mouse_pressed (mouse_key, pos);
        if (is_released_on_child)
        {
            return true;
        }
    }
    
    if (Window::on_mouse_pressed (mouse_key, pos))
        return true;

    if (menu_->on_mouse_pressed (mouse_key, pos_))
    {
        return true;
    }

    return false;   
}

bool Master_window::on_mouse_released (Mouse_key mouse_key, Vector &pos) 
{
    Vector pos_ = transform_.apply_transform (pos);

    for (int window_idx = 0; window_idx < windows.size; ++window_idx)
    {
        Window *window = (Window *)list_get (&windows, window_idx + 1);
        if (!window)
        {
            fprintf (stderr, "Event error: nil button is detected.\n"
                             "Hint: nil button idx in windows list = %d.\n"
                             "Hint: windows list size = %d.\n", window_idx + 1, windows.size);
            return false;
        }
        assert (window);

        bool is_released_on_child = window->on_mouse_released (mouse_key, pos);
        if (is_released_on_child)
        {
            return true;
        }
    }

    if (Window::on_mouse_released (mouse_key, pos))
    {
        return true;
    }

    if (menu_->on_mouse_released (mouse_key, pos_))
    {
        return true;
    }

    return false;   
}

bool Master_window::on_mouse_moved (Vector &new_pos) 
{
    Vector pos_ = transform_.apply_transform (new_pos);

    for (int window_idx = 0; window_idx < windows.size; ++window_idx)
    {
        Window *window = (Window *)list_get (&windows, window_idx + 1);
        if (!window)
        {
            fprintf (stderr, "Event error: nil button is detected.\n"
                             "Hint: nil button idx in windows list = %d.\n"
                             "Hint: windows list size = %d.\n", window_idx + 1, windows.size);
            return false;
        }
        assert (window);

        bool is_released_on_child = window->on_mouse_moved (new_pos);
        if (is_released_on_child)
        {
            return true;
        }
    }

    if (Window::on_mouse_moved (new_pos))
        return true;

    if (menu_->on_mouse_moved (pos_))
    {
        return true;
    }

    return false;
}   

bool Master_window::on_keyboard_pressed  (Keyboard_key key) 
{
    bool status = false;

    for (int window_idx = 0; window_idx < windows.size; ++window_idx)
    {
        Window *window = (Window *)list_get (&windows, window_idx + 1);
        if (!window)
        {
            fprintf (stderr, "Event error: nil button is detected.\n"
                             "Hint: nil button idx in windows list = %d.\n"
                             "Hint: windows list size = %d.\n", window_idx + 1, windows.size);
            return false;
        }
        assert (window);

        status |= window->on_keyboard_pressed (key);
    }

    status |= Window::on_keyboard_pressed (key);
    status |= menu_->on_keyboard_pressed (key);

    return status;   
}

bool Master_window::on_keyboard_released (Keyboard_key key) 
{
    bool status = false;

    for (int window_idx = 0; window_idx < windows.size; ++window_idx)
    {
        Window *window = (Window *)list_get (&windows, window_idx + 1);
        if (!window)
        {
            fprintf (stderr, "Event error: nil button is detected.\n"
                             "Hint: nil button idx in windows list = %d.\n"
                             "Hint: windows list size = %d.\n", window_idx + 1, windows.size);
            return false;
        }
        assert (window);

        status |= window->on_keyboard_released (key);
    }

    status |= Window::on_keyboard_released (key);
    status |= menu_->on_keyboard_released (key);

    return status;   
}

bool Master_window::on_time (float delta_sec)
{
    bool status = false;
    
    for (int window_idx = 0; window_idx < windows.size; ++window_idx)
    {
        Window *window = (Window *)list_get (&windows, window_idx + 1);
        if (!window)
        {
            fprintf (stderr, "Event error: nil button is detected.\n"
                             "Hint: nil button idx in windows list = %d.\n"
                             "Hint: windows list size = %d.\n", window_idx + 1, windows.size);
            return false;
        }
        assert (window);

        status |= window->on_time (delta_sec);
    }

    status |= Window::on_time (delta_sec);
    status |= menu_->on_time (delta_sec);
    
    return status;
}