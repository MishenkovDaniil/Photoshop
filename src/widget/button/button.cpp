#include <iostream>
#include <cassert>
#include <SFML/Graphics.hpp>

#include "button.h"

// may be made base class for text and image to add it as parameter in button and use int it whether image or text
//image of our size-->texture-->sprite-->sprite.set_texture () and sprite.set_texture_rect ()

Button::Button (Vector lh_corner, int width, int height, Button_run_fn func, void *controlled_widget, void *arg, Color fill_color, int run_mask) :
            transform_ (Transform (lh_corner)),
            width_ (width),
            height_ (height),
            run_fn_ (func),
            fill_color_ (fill_color),
            controlled_widget_ (controlled_widget),
            run_mask_ (run_mask),
            arg_ (arg), 
            press_pos_ (Vector ()) {};

bool Button::contains (double x, double y) const
{
    return (0 <= x && 0 <= y &&
            x <= width_ && y <= height_);
}

bool Button::run ()
{
    if (!run_fn_)
    {
        fprintf (stderr, "Error: nullptr function pointer in Button.\n");
        return false;
    }
    
    return run_fn_ (controlled_widget_, arg_);
}

void Button::render (sf::RenderTarget &target, M_vector<Transform> &transform_stack)
{
    Transform top = transform_stack.get_size () > 0 ? transform_stack.top () : Transform (Vector (0, 0));
    Transform unite = transform_.unite (top);

    Vector lh_corner = unite.offset_;

    sf::RectangleShape rect (Vector (width_, height_));
    rect.setFillColor (fill_color_);
/*TODO: 
//make constant for thickness and add possibility to change it (in constructor for example)*/
    rect.setOutlineThickness (1); 
    rect.setPosition (lh_corner.get_x (), lh_corner.get_y ());

    target.draw (rect);
}   

bool Button::on_mouse_pressed  (Mouse_key mouse_key, Vector &pos)
{
    Vector pos_ = transform_.apply_transform (pos);

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

bool Button::on_mouse_released (Mouse_key mouse_key, Vector &pos)
{ 
    Vector pos_ = transform_.apply_transform (pos);

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

bool Button::on_mouse_moved    (Vector &new_pos)
{ 
    Vector pos_ = transform_.apply_transform (new_pos);
    static int call_num = 1;
    
    if ((run_mask_ & MOVE_BUTTON) && is_pressed_)
    {
        Vector change (0, pos_.get_y () - press_pos_.get_y ());

        transform_.offset_ += change;
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

bool Button::on_time (float delta_sec)
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

void Texture_button::render (sf::RenderTarget &target, M_vector<Transform> &transform_stack)
{
    Transform top = transform_stack.get_size () > 0 ? transform_stack.top () : Transform (Vector (0, 0));
    Transform unite = transform_.unite (top);

    Vector lh_corner = unite.offset_;

    sprite_->setTexture (*cur_texture_);
    sprite_->setPosition (lh_corner.get_x (), lh_corner.get_y ());

    target.draw (*sprite_);
}

bool Texture_button::on_mouse_pressed  (Mouse_key mouse_key, Vector &pos)
{
    bool status = Button::on_mouse_pressed (mouse_key, pos);
    cur_texture_ = is_pressed_ ? &pressed_texture_ : &released_texture_;
    return status;
}

bool Texture_button::on_mouse_released (Mouse_key mouse_key, Vector &pos)
{
    bool status = Button::on_mouse_released (mouse_key, pos);
    cur_texture_ = is_pressed_ ? &pressed_texture_ : &released_texture_;
    return status;
}

bool Texture_button::on_mouse_moved    (Vector &new_pos)
{
    bool status = Button::on_mouse_moved (new_pos);
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


// void String_button::draw (sf::RenderWindow &window, int screen_w, int screen_h)const
// {
//     int width = rl_corner_.x_ - lh_corner_.x_;
//     int height = rl_corner_.y_ - lh_corner_.y_;

//     sf::Texture button_texture;
//     sf::RectangleShape button (sf::Vector2f (width, height));
//     is_pressed_ ? button.setFillColor((sf::Color)pressed_color_) : button.setFillColor ((sf::Color)*cur_color_);//may be setfillcolor (cur_color_) ???
//                     // button.setOutlineColor (sf::Color::White);
//                        button.setOutlineThickness (1);
//                        button.setTexture (&button_texture);
//                        button.setPosition (lh_corner_.x_, lh_corner_.y_);
    
//     sf::Text text;
//     sf::Font font;
//     font.loadFromFile ("buttons/button_font.ttf");
//     text.setString (string_);
//     text.setFont (font);
//     text.setFillColor (sf::Color::White);
//     text.setCharacterSize (10);

//     double text_width = text.findCharacterPos(str_size - 1).x - text.findCharacterPos (0).x;
//     text.setPosition (lh_corner_.x_ + (width - text_width) / 2, lh_corner_.y_ + height / 2 - 5);
    
//     window.draw (button);
//     window.draw (text);
// }

// void String_button::update (bool is_pressed)
// {
//     if (is_pressed == true)
//         cur_color_ = &pressed_color_;
//     else 
//         cur_color_ = &released_color_;

//     is_pressed_ = is_pressed;
// }