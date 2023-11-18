#include "header.h"

#include <cstring>

Header::Header (Vec2d lh_pos, int width, const char *string, Window *window, Color background) : 
    // transform_ (Transform (lh_pos)),
    width_ (width),
    parent_window_ (window),
    background_color (background)
{
    layout_ = new Default_layout_box (lh_pos, Vec2d (width, HEADER_HEIGHT));
    assert (layout_);

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
    delete layout_;
}

void Header::render (sf::RenderTarget &target, TransformStack &transform_stack)
{
    transform_stack.enter (Transform (layout_->get_position ()));

    Vec2d lh_pos = transform_stack.top ().getOffset ();

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

    transform_stack.leave ();
}


void Header::onMousePressed     (MousePressedEvent &event, EHC &ehc)
{
    Transform tr (layout_->get_position ());
    Transform unite = tr.combine (ehc.stack.top ());
    Vec2d pos_ = unite.apply (event.pos);

    if (contains (pos_.get_x (), pos_.get_y ()))
    {
        is_moving_ = true;
        move_start_ = event.pos;
        ehc.stopped = true;
    }
}

void Header::onMouseReleased    (MouseReleasedEvent &event, EHC &ehc)
{
    if (is_moving_)
    {
        is_moving_ = false;
    }
}   

void Header::onMouseMove        (MouseMoveEvent &event, EHC &ehc)
{
    if (is_moving_)
    {
        Vec2d move = event.pos - move_start_;
        // parent_window_->get_transform ().offset_ += move;
        Layout_box &layout = parent_window_->get_layout_box ();
        layout.set_position (layout.get_position () + move);
        parent_window_->set_layout_box (layout);
        move_start_ = event.pos;

        ehc.stopped = true;
    }
}

void Header::onKeyboardPressed  (KeyboardPressedEvent &event, EHC &ehc)
{
    return;
}

void Header::onKeyboardReleased (KeyboardReleasedEvent &event, EHC &ehc)
{
    return;
}

void Header::onTick             (TickEvent &event, EHC &ehc)
{
    return;
}


bool Header::contains (int x, int y)
{
    if ((x >= 0 && x <= width_) &&
        (y >= 0 && y <= HEADER_HEIGHT))
        return true;
    return false;
}