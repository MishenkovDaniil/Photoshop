#include "header.h"

#include <cstring>

Header::Header (Vector lh_pos, int width, const char *string, Window *window, Color background) : 
    transform_ (Transform (lh_pos)),
    width_ (width),
    parent_window_ (window),
    background_color (background)
{
    if (string)
    {
        str_len = strlen (string);
        
        string_ = new char [str_len + 1];
        assert (string_);

        strncpy (string_, string, str_len + 1);
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

void Header::render (sf::RenderTarget &target, M_vector<Transform> &transform_stack)
{
    Transform top = transform_stack.get_size () > 0 ? transform_stack.top () : Transform (Vector (0, 0));
    Transform unite = transform_.unite (top);
    transform_stack.push (unite);

    Vector lh_pos = transform_stack.top ().offset_;

    sf::RectangleShape header (sf::Vector2f (width_, HEADER_HEIGHT));
                       header.setFillColor((sf::Color)background_color); 
                       header.setOutlineColor (Color (50, 50, 50));
                       header.setOutlineThickness (-1);
                       header.setPosition (lh_pos);
    
    sf::Text text;
    sf::Font font;
    font.loadFromFile (font_file_);
    text.setString (string_);
    text.setFont (font);
    text.setFillColor (TEXT_COLOR);
    text.setCharacterSize (CHARACTER_SIZE);

    double text_width = text.findCharacterPos(str_len - 1).x - text.findCharacterPos (0).x;
    text.setPosition (lh_pos.get_x () + (width_ - text_width) / 2, lh_pos.get_y () + HEADER_HEIGHT / 2 - CHARACTER_SIZE / 2);

    target.draw (header);
    target.draw (text);

    transform_stack.pop ();
}

bool Header::on_mouse_pressed  (Mouse_key mouse_key, Vector &pos, M_vector<Transform> &transform_stack)
{
    Transform top = transform_stack.get_size () > 0 ? transform_stack.top () : Transform (Vector (0, 0));
    Transform unite = transform_.unite (top);
    
    Vector pos_ = unite.apply_transform (pos);

    if (contains (pos_.get_x (), pos_.get_y ()))
    {
        is_moving_ = true;
        move_start_ = pos;
        return true;
    }

    return false;
}

bool Header::on_mouse_released (Mouse_key mouse_key, Vector &pos, M_vector<Transform> &transform_stack)
{
    if (is_moving_)
    {
        is_moving_ = false;
        return true;
    }

    return false;
}

bool Header::on_mouse_moved    (Vector &new_pos, M_vector<Transform> &transform_stack)
{
    if (is_moving_)
    {
        Vector move = new_pos - move_start_;
        parent_window_->get_transform ().offset_ += move;
        move_start_ = new_pos;
        return true;
    }

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


bool Header::contains (int x, int y)
{
    if ((x >= 0 && x <= width_) &&
        (y >= 0 && y <= HEADER_HEIGHT))
        return true;
    return false;
}