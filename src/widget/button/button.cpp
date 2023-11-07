#include <iostream>
#include <cassert>
#include <SFML/Graphics.hpp>

#include "button.h"

// may be made base class for text and image to add it as parameter in button and use int it whether image or text
//image of our size-->texture-->sprite-->sprite.set_texture () and sprite.set_texture_rect ()

Button::Button (Vector lh_corner, int width, int height, Button_run_fn func, void *controlled_widget, void *arg, Color fill_color, int run_mask) :
            // transform_ (Transform (lh_corner)),
            width_ (width),
            height_ (height),
            run_fn_ (func),
            fill_color_ (fill_color),
            controlled_widget_ (controlled_widget),
            run_mask_ (run_mask),
            arg_ (arg), 
            press_pos_ (Vector ()) 
{
    layout_ = new Default_layout_box (lh_corner, Vector (width, height));
    assert (layout_);
};

bool Button::contains (double x, double y) const
{
    Vector size = layout_->get_size ();

    return (0 <= x && 0 <= y &&
            x <= size.get_x () && y <= size.get_y ());
}

bool Button::run ()
{
    if (!run_fn_)
    {
        fprintf (stderr, "Warning: nullptr function pointer in Button.\n");
        return false;
    }
    
    return run_fn_ (controlled_widget_, arg_);
}

void Button::render (sf::RenderTarget &target, Transform_stack &transform_stack)
{
    Transform tr (layout_->get_position ());
    Transform unite = tr.unite (transform_stack.top ());
    Vector lh_corner = unite.offset_;
    Vector size = layout_->get_size ();

    sf::RectangleShape rect (unite.scale_apply(size));
    rect.setFillColor (fill_color_);
/*TODO: 
//make constant for thickness and add possibility to change it (in constructor for example)*/
    rect.setOutlineThickness (1); 
    rect.setPosition (lh_corner.get_x (), lh_corner.get_y ());

    target.draw (rect);
}   

bool Button::on_mouse_pressed  (Mouse_key mouse_key, Vector &pos, Transform_stack &transform_stack)
{
    Transform tr (layout_->get_position ());
    Transform unite = tr.unite (transform_stack.top ());
    Vector pos_ = unite.apply_transform (pos);

    if (contains (pos_.get_x (), pos_.get_y ()))
    {
        if (run_mask_ & PRESS_BUTTON)
        {
            return run ();
        }
        press_pos_ = pos_;
        is_pressed_ = true;
        return true;
    }

    return false;
} 

bool Button::on_mouse_released (Mouse_key mouse_key, Vector &pos, Transform_stack &transform_stack)
{ 
    Transform tr (layout_->get_position ());
    Transform unite = tr.unite (transform_stack.top ());
    Vector pos_ = unite.apply_transform (pos);

    if (contains (pos_.get_x (), pos_.get_y ()))
    {
        if (run_mask_ & RELEASE_BUTTON)
        {
            return run ();
        }
    }

    if (is_pressed_)
    {
        is_pressed_ = false;
        return true;
    }

    return false;
} 

bool Button::on_mouse_moved    (Vector &new_pos, Transform_stack &transform_stack)
{ 
    Transform tr (layout_->get_position ());
    Transform unite = tr.unite (transform_stack.top ());
    Vector pos_ = unite.apply_transform (new_pos);
    
    if ((run_mask_ & MOVE_BUTTON) && is_pressed_)
    {
        Vector change (0, pos_.get_y () - press_pos_.get_y ());
        layout_->set_position (layout_->get_position () + change);

        return true;
    }

    return false;
}   

bool Button::on_keyboard_pressed  (Keyboard_key key)
{ 
    // TODO
    return false;
}   

bool Button::on_keyboard_released (Keyboard_key key)
{
    // TODO
    return false;
}   

bool Button::on_tick (float delta_sec)
{
    // TODO
    return false;
}   


Texture_button::Texture_button (Vector lh_corner, int width, int height, sf::Texture &pressed, sf::Texture &released, 
                                Button_run_fn func, void *controlled_widget, void *arg, int run_mask) : 
    Button (lh_corner, width, height, func, controlled_widget, arg, Color (), run_mask),
    pressed_texture_ (pressed),
    released_texture_ (released) 
{
    cur_texture_ = &released_texture_;
    sprite_ = new sf::Sprite;
}

Texture_button::~Texture_button ()
{
    if (sprite_)
        delete sprite_;
}

void Texture_button::render (sf::RenderTarget &target, Transform_stack &transform_stack)
{
    Transform tr (layout_->get_position ());
    Transform unite = tr.unite (transform_stack.top ());
    Vector lh_corner = unite.offset_;

    sprite_->setTexture (*cur_texture_);
    sprite_->setPosition (lh_corner.get_x (), lh_corner.get_y ());

    target.draw (*sprite_);
}

bool Texture_button::on_mouse_pressed  (Mouse_key mouse_key, Vector &pos, Transform_stack &transform_stack)
{
    bool status = Button::on_mouse_pressed (mouse_key, pos, transform_stack);
    cur_texture_ = is_pressed_ ? &pressed_texture_ : &released_texture_;
    return status;
}

bool Texture_button::on_mouse_released (Mouse_key mouse_key, Vector &pos, Transform_stack &transform_stack)
{
    bool status = Button::on_mouse_released (mouse_key, pos, transform_stack);
    cur_texture_ = is_pressed_ ? &pressed_texture_ : &released_texture_;
    return status;
}

bool Texture_button::on_mouse_moved    (Vector &new_pos, Transform_stack &transform_stack)
{
    bool status = Button::on_mouse_moved (new_pos, transform_stack);
    cur_texture_ = is_pressed_ ? &pressed_texture_ : &released_texture_;
    return status;
} 

bool Texture_button::on_keyboard_pressed  (Keyboard_key key)
{
    bool status = Button::on_keyboard_pressed (key);
    cur_texture_ = is_pressed_ ? &pressed_texture_ : &released_texture_;
    return status;
}   

bool Texture_button::on_keyboard_released (Keyboard_key key)
{
    bool status = Button::on_keyboard_released (key);
    cur_texture_ = is_pressed_ ? &pressed_texture_ : &released_texture_;
    return status;
}   


String_button::String_button (Vector lh_corner, int width, int height, const char *string, const Color &pressed_color, const Color &released_color, Button_run_fn func, void *controlled_widget,
               void *arg, int run_mask) :
    Button (lh_corner, width, height, func, controlled_widget, arg, Transparent, run_mask),
    pressed_color_  (pressed_color), 
    released_color_ (released_color)
{
    cur_color_ = &released_color_;

    assert (string);

    str_size_ = std::strlen(string);

    string_ = new char[str_size_ + 1];
    assert (string_);
    std::strcpy (string_, string); 
}

String_button::~String_button () 
{
    if (string_) delete[] string_;
}

void String_button::render (sf::RenderTarget &target, Transform_stack &transform_stack)
{
    assert (layout_);

    transform_stack.push (Transform (layout_->get_position ()));
    Vector lh_pos = transform_stack.top ().offset_;

    Vector size = layout_->get_size ();
    Vector real_size = transform_stack.top ().scale_apply (size);
    sf::RectangleShape button (real_size);
                       button.setFillColor (*cur_color_);
                       button.setOutlineThickness (1);
                       button.setPosition (lh_pos.get_x (), lh_pos.get_y ());
    sf::Text text;
    sf::Font font;
    font.loadFromFile (DEFAULT_FONT_FILE);
    text.setString (string_);
    text.setFont (font);
    text.setFillColor (TEXT_COLOR);
    text.setCharacterSize (BUTTON_TEXT_SIZE);

    double text_width = text.findCharacterPos(str_size_ - 1).x - text.findCharacterPos (0).x;
    text.setPosition (lh_pos.get_x () + (real_size.get_x () - text_width) / 2, lh_pos.get_y () + real_size.get_y () / 2 - BUTTON_TEXT_SIZE / 2);
    
    target.draw (button);
    target.draw (text);

    transform_stack.pop ();
}

bool String_button::on_mouse_pressed  (Mouse_key mouse_key, Vector &pos, Transform_stack &transform_stack)
{
    bool status = Button::on_mouse_pressed (mouse_key, pos, transform_stack);
    cur_color_ = is_pressed_ ? &pressed_color_ : &released_color_;
    return status;
}

bool String_button::on_mouse_released (Mouse_key mouse_key, Vector &pos, Transform_stack &transform_stack)
{
    bool status = Button::on_mouse_released (mouse_key, pos, transform_stack);
    cur_color_ = is_pressed_ ? &pressed_color_ : &released_color_;
    return status;
}

bool String_button::on_mouse_moved    (Vector &new_pos, Transform_stack &transform_stack)                 
{
    bool status = Button::on_mouse_moved (new_pos, transform_stack);
    cur_color_ = is_pressed_ ? &pressed_color_ : &released_color_;
    return status;
}

bool String_button::on_keyboard_pressed  (Keyboard_key key)             
{
    bool status = Button::on_keyboard_pressed (key);
    cur_color_ = is_pressed_ ? &pressed_color_ : &released_color_;
    return status;
}

bool String_button::on_keyboard_released (Keyboard_key key)             
{
    bool status = Button::on_keyboard_released (key);
    cur_color_ = is_pressed_ ? &pressed_color_ : &released_color_;
    return status;
}


List_button::List_button (Button *list_button) :
    list_button_ (list_button)
{
    assert (list_button);
    layout_ = new Default_layout_box (Vector (list_button->layout_->get_position () + Vector (0, list_button->layout_->get_size ().get_y ())), 
                                              list_button->layout_->get_size ());
    assert (layout_);

}

List_button::~List_button ()
{
    if (layout_) delete (Default_layout_box *)layout_;
    layout_ = nullptr;
}

void List_button::add_button (Button *button)
{
    assert (button);
    Layout_box *old_layout = button->layout_;
    assert (old_layout);

    size_t button_height = old_layout->get_size ().get_y ();
    delete old_layout;
    
    Default_layout_box *new_layout = new Default_layout_box (Vector (0, relative_height_), Vector (layout_->get_size ().get_x (), button_height));
    relative_height_ += button_height;
    button->set_layout_box (*new_layout);
    
    buttons_.add (button);
}


void List_button::render (sf::RenderTarget &target, Transform_stack &transform_stack)
{
    list_button_->render (target, transform_stack);

    if (is_open_)
    {
        transform_stack.push (Transform (layout_->get_position ()));

        size_t button_num = buttons_.get_size ();
        for (int button_idx = 0; button_idx < button_num; ++button_idx)
        {
            Button *button = buttons_[button_idx];
            if (!button)
            {
                fprintf (stderr, "Event error: nil button is detected.\n"
                                "Hint: nil button idx in buttons vector = %d.\n"
                                "Hint: buttons vector size = %lu.\n", button_idx, button_num);
                return;
            }
            assert (button);

            button->render (target, transform_stack);
        }

        transform_stack.pop ();
    }
}   

bool List_button::on_mouse_pressed     (Mouse_key mouse_key, Vector &pos, Transform_stack &transform_stack)
{
    assert (layout_);
    
    bool status = false;

    Transform list_button_tr (list_button_->layout_->get_position ());
    Transform unite = list_button_tr.unite (transform_stack.top ());
    Vector pos_ = unite.apply_transform (pos);

    status |= list_button_->on_mouse_pressed (mouse_key, pos, transform_stack);
    
    if (list_button_->contains (pos_.get_x (), pos_.get_y ()))
    {
        is_open_ = !is_open_;
        return true;
    }
    
    if (!is_open_)
    {
        return status;
    }

    transform_stack.push (Transform (layout_->get_position ()));
    
    size_t button_num = buttons_.get_size ();

    for (int button_idx = 0; button_idx < button_num; ++button_idx)
    {
        Button *button = buttons_[button_idx];
        if (!button)
        {
            fprintf (stderr, "Event error: nil button is detected.\n"
                            "Hint: nil button idx in buttons vector = %d.\n"
                            "Hint: buttons vector size = %lu.\n", button_idx, button_num);
            return false;
        }
        assert (button);

        status |= button->on_mouse_pressed (mouse_key, pos, transform_stack);
    }

    transform_stack.pop ();
    
    if ((!status))
    {
        is_open_ = !is_open_;
        status = true;
    }

    return status;
}

bool List_button::on_mouse_released    (Mouse_key mouse_key, Vector &pos, Transform_stack &transform_stack)
{
    bool status = false;
    
    status |= list_button_->on_mouse_released (mouse_key, pos, transform_stack);

    if (is_open_)
    {
        transform_stack.push (Transform (layout_->get_position ()));

        Vector lh_pos = transform_stack.top ().offset_;

        size_t button_num = buttons_.get_size ();
        for (int button_idx = 0; button_idx < button_num; ++button_idx)
        {
            Button *button = buttons_[button_idx];
            if (!button)
            {
                fprintf (stderr, "Event error: nil button is detected.\n"
                                "Hint: nil button idx in buttons vector = %d.\n"
                                "Hint: buttons vector size = %lu.\n", button_idx, button_num);
                return false;
            }
            assert (button);

            status |= button->on_mouse_released (mouse_key, lh_pos, transform_stack);
        }

        transform_stack.pop ();
    }

    return status;
}

bool List_button::on_mouse_moved       (Vector &new_pos, Transform_stack &transform_stack)
{
    bool status = false;
    
    status |= list_button_->on_mouse_moved (new_pos, transform_stack);

    if (is_open_)
    {
        transform_stack.push (Transform (layout_->get_position ()));

        Vector lh_pos = transform_stack.top ().offset_;

        size_t button_num = buttons_.get_size ();
        for (int button_idx = 0; button_idx < button_num; ++button_idx)
        {
            Button *button = buttons_[button_idx];
            if (!button)
            {
                fprintf (stderr, "Event error: nil button is detected.\n"
                                "Hint: nil button idx in buttons vector = %d.\n"
                                "Hint: buttons vector size = %lu.\n", button_idx, button_num);
                return false;
            }
            assert (button);

            status |= button->on_mouse_moved (lh_pos, transform_stack);
        }

        transform_stack.pop ();
    }

    return status;
}   

bool List_button::on_keyboard_pressed  (Keyboard_key key)
{
    return false;

}   

bool List_button::on_keyboard_released (Keyboard_key key)
{
    return false;

}   

bool List_button::on_tick (float delta_sec)
{
    return false;
}   
