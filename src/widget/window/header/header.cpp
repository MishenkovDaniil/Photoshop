#include "header.h"

#include <cstring>

Header::Header (Vector lh_pos, int width, const char *string, Color background) : 
    lh_pos_ (lh_pos),
    width_ (width),
    background_color (background)
{
    if (string)
    {
        int str_len = strlen (string);
        
        string_ = new char [str_len + 1];
        assert (string_);

        strncpy (string_, string, str_len);
    }
}

Header::~Header ()
{
    if (string_) 
    {
        delete[] string_;
        string_ = nullptr;
    }
}

void Header::render (sf::RenderTarget &target) const
{
    // sf::Texture button_texture;
    sf::RectangleShape header (sf::Vector2f (width_, height_));
                       header.setFillColor((sf::Color)background_color); 
                    // button.setOutlineColor (sf::Color::White);
                    // button.setOutlineThickness (1);
                    // button.setTexture (&button_texture);
                       header.setPosition (lh_pos_);
    
    sf::Text text;
    sf::Font font;
    font.loadFromFile (font_file_);
    text.setString (string_);
    text.setFont (font);
    text.setFillColor (Color (0,0,0));
    text.setCharacterSize (10);

    double text_width = text.findCharacterPos(str_len - 1).x - text.findCharacterPos (0).x;
    text.setPosition (lh_pos_.get_x () + (width_ - text_width) / 2, lh_pos_.get_y () + height_ / 2 - 5);
    
    target.draw (header);
    target.draw (text);
}

bool Header::on_mouse_pressed  (Mouse_key mouse_key, Vector &pos)
{
    ///TODO
    return false;
}

bool Header::on_mouse_released (Mouse_key mouse_key, Vector &pos)
{
    ///TODO
    return false;
}

bool Header::on_mouse_moved    (Vector &new_pos)
{
    ///TODO
    return false;
}   

bool Header::on_keyboard_pressed  (Keyboard_key key)
{
    ///TODO
    return false;
}

bool Header::on_keyboard_released (Keyboard_key key)
{
    ///TODO
    return false;
}

bool Header::on_time (float delta_sec)
{
    ///TODO
    return false;
}