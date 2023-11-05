#include "menu.h"


Menu::Menu (Vector lh_pos, int width, int height) :
    // transform_ (Transform (lh_pos)),
    width_ (width),
    height_ (height)
{
    layout_ = new Default_layout_box (lh_pos, Vector (width, height));
    assert (layout_);

    list_ctor (&buttons, MENU_INIT_CAPACITY);
}

Menu::~Menu ()
{
    list_dtor (&buttons);
    delete layout_;
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

void Menu::render (sf::RenderTarget &target, Transform_stack &transform_stack) 
{
    transform_stack.push (Transform (layout_->get_position ()));

    Vector lh_pos = transform_stack.top ().offset_;

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

bool Menu::on_mouse_pressed  (Mouse_key mouse_key, Vector &pos, Transform_stack &transform_stack)
{
    transform_stack.push (Transform (layout_->get_position ()));

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

        if (button->on_mouse_pressed (mouse_key, pos, transform_stack))
        {
            status = true;
        }
    }

    transform_stack.pop ();

    return status;
} 

bool Menu::on_mouse_released (Mouse_key mouse_key, Vector &pos, Transform_stack &transform_stack)
{
    transform_stack.push (Transform (layout_->get_position ()));

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

        if (button->on_mouse_released (mouse_key, pos, transform_stack))
        {
            status = true;
        }
    }

    transform_stack.pop ();

    return status;
} 

bool Menu::on_mouse_moved    (Vector &new_pos, Transform_stack &transform_stack) //think about hovering that can be done to more than one button at the moment  (one hovers, one antihovers)
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
