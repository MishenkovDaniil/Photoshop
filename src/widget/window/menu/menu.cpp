#include "menu.h"


Menu::Menu (Vector lh_pos, int width) :
    lh_pos_ (lh_pos),
    width_ (width)
{
    list_ctor (&buttons, MENU_INIT_CAPACITY);
}

Menu::~Menu ()
{
    list_dtor (&buttons);
}


void Menu::add_button (Button *button)
{
    assert (button && "nullptr button added to menu");
    if (button->get_height () != height_)
    {
        fprintf (stderr, "Error:wrong menu button height.\n"
                        "Hint: button height %d, expected %d.\n", button->get_height (), height_);
        return;
    }
    list_insert (&buttons, 0, button);
}

void Menu::render (sf::RenderTarget &target) const 
{
    sf::RectangleShape rect (Vector (width_, height_));
    rect.setFillColor (Color (100, 100, 100,255));
    rect.setPosition (lh_pos_);
    target.draw (rect);

    for (int button_idx = 0; button_idx < buttons.size; ++button_idx)
    {
        Button *button = (Button *)list_get (&buttons, button_idx + 1);
        if (!button)
        {
            fprintf (stderr, "Event error: nil button is detected.\n"
                             "Hint: nil button idx in buttons list = %d.\n"
                             "Hint: buttons list size = %d.\n", button_idx + 1, buttons.size);
            return;
        }
        assert (button);

        button->render (target);
    }
}

bool Menu::on_mouse_pressed  (Mouse_key mouse_key, Vector &pos)
{
    //TODO...
    return false;
} 

bool Menu::on_mouse_released (Mouse_key mouse_key, Vector &pos)
{
    //TODO...
    return false;
} 

bool Menu::on_mouse_moved    (Vector &new_pos)
{
    //TODO...
    return false;
}   
  /// x, y - absolute values 
bool Menu::on_keyboard_pressed  (Keyboard_key key)
{
    //TODO...
    return false;
} 

bool Menu::on_keyboard_released (Keyboard_key key)
{
    //TODO...
    return false;
} 

bool Menu::on_time (float delta_sec)
{
    //TODO...
    return false;
}
