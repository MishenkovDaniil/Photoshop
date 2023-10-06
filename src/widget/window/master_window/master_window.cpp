#include "master_window.h"

Master_window::Master_window (int width, int height, Vector lh_pos, const char *w_name, int list_capacity) :
    Window (width, height, lh_pos, w_name)
{   
    menu_ = new Menu(lh_pos_ + Vector (0, HEADER_HEIGHT), width);
    assert (menu_);
}


void Master_window::add_menu_button (Button *button)
{
    menu_->add_button (button);
}

void Master_window::render (sf::RenderTarget &target) const 
{
    Window::render (target);
    menu_->render (target);

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

        window->render (target);
    }
}

bool Master_window::on_mouse_pressed  (Mouse_key mouse_key, Vector &pos) 
{
    ///TODO
    return false;   
}

bool Master_window::on_mouse_released (Mouse_key mouse_key, Vector &pos) 
{
    ///TODO
    return false;   
}

bool Master_window::on_mouse_moved    (Vector &new_pos) 
{
    ///TODO
    return false;
}   

bool Master_window::on_keyboard_pressed  (Keyboard_key key) 
{
    ///TODO
    return false;   
}

bool Master_window::on_keyboard_released (Keyboard_key key) 
{
    ///TODO
    return false;   
}

bool Master_window::on_time (float delta_sec)
{
    ///TODO
    return false;
}