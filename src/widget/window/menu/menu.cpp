#include "menu.h"


Menu::Menu (Vector lh_pos, int width, int height) :
    transform_ (Transform (lh_pos)),
    width_ (width),
    height_ (height)
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
    // if (button->get_height () != height_)
    // {
    //     fprintf (stderr, "Error:wrong menu button height.\n"
    //                     "Hint: button height %d, expected %d.\n", button->get_height (), height_);
    //     return;
    // }
    list_insert (&buttons, 0, button);
}

void Menu::render (sf::RenderTarget &target, M_vector<Transform> &transform_stack) 
{
    Transform top = transform_stack.get_size () > 0 ? transform_stack.top () : Transform (Vector (0, 0));
    Transform unite = transform_.unite (top);
    transform_stack.push (unite);

    Vector lh_pos = unite.offset_;

    sf::RectangleShape rect (Vector (width_, height_));
    rect.setFillColor    (Color (200, 200, 200));
    rect.setOutlineColor (Color (50, 50, 50));
    rect.setOutlineThickness (-1);
    rect.setPosition (lh_pos + Vector (0, -1));
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

        button->render (target, transform_stack);
    }

    transform_stack.pop ();
}

bool Menu::on_mouse_pressed  (Mouse_key mouse_key, Vector &pos)
{
    Vector pos_ = transform_.apply_transform (pos);
    bool status = false;

    for (int button_idx = 0; button_idx < buttons.size; ++button_idx)
    {
        Button *button = (Button *)list_get (&buttons, button_idx + 1);
        if (!button)
        {
            fprintf (stderr, "Event error: nil button is detected.\n"
                             "Hint: nil button idx in buttons list = %d.\n"
                             "Hint: buttons list size = %d.\n", button_idx + 1, buttons.size);
            return false;
        }
        assert (button);

        if (button->on_mouse_pressed (mouse_key, pos_))
        {
            status = true;
        }
    }

    return status;
} 

bool Menu::on_mouse_released (Mouse_key mouse_key, Vector &pos)
{
    Vector pos_ = transform_.apply_transform (pos);
    bool status = false;
    
    for (int button_idx = 0; button_idx < buttons.size; ++button_idx)
    {
        Button *button = (Button *)list_get (&buttons, button_idx + 1);
        if (!button)
        {
            fprintf (stderr, "Event error: nil button is detected.\n"
                             "Hint: nil button idx in buttons list = %d.\n"
                             "Hint: buttons list size = %d.\n", button_idx + 1, buttons.size);
            return false;
        }
        assert (button);

        if (button->on_mouse_released (mouse_key, pos_))
        {
            status = true;
        }
    }

    return status;
} 

bool Menu::on_mouse_moved    (Vector &new_pos) //think about hovering that can be done to more than one button at the moment  (one hovers, one antihovers)
                                                // connect hovering with 'on_time ()'
{
    //TODO...
    return false;
}   

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
