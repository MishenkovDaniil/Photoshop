#include "header.h"

#include <cstring>

Header::Header (plug::Vec2d lh_pos, int width, const char *string, Window *window, plug::Color background) : 
    width_ (width),
    parent_window_ (window),
    background_color (background), 
    close_button (new Texture_button (plug::Vec2d (width - HEADER_HEIGHT, 0), HEADER_HEIGHT, HEADER_HEIGHT, 
                                      CLOSE_BUTTON_SPRITE, CLOSE_BUTTON_SPRITE, close_window, window, nullptr, PRESS_BUTTON))
{
    layout_ = new Default_layout_box (lh_pos, plug::Vec2d (width, HEADER_HEIGHT));
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
    if (close_button)
    {
        delete close_button;
        close_button = nullptr;
    }
    delete layout_;
    layout_ = nullptr;
}

void Header::draw (plug::TransformStack &transform_stack, plug::RenderTarget &target)
{
    transform_stack.enter (plug::Transform (layout_->getPosition ()));

    plug::Vec2d lh_pos = transform_stack.top ().getOffset ();

    RectangleShape     header (plug::Vec2d (width_, HEADER_HEIGHT));
                       header.setFillColor(background_color); 
                       header.setOutlineColor (plug::Color (50, 50, 50));
                       header.setOutlineThickness (-1);
                       header.setPosition (lh_pos.x, lh_pos.y);
    Text text;
    Font font;
    font.loadFromFile (font_file_);
    text.setString (string_);
    text.setFont (font);
    text.setFillColor (TEXT_COLOR);
    text.setCharacterSize (CHARACTER_SIZE);

    double text_width = text.findCharacterPos(str_len - 1).x - text.findCharacterPos (0).x;
    text.setPosition (lh_pos.get_x () + (width_ - text_width) / 2, lh_pos.get_y () + HEADER_HEIGHT / 2 - CHARACTER_SIZE / 2);

    ((RenderTexture &)target).draw (header);
    ((RenderTexture &)target).draw (text);

    close_button->draw (transform_stack, target);
    transform_stack.leave ();
}

void Header::onMousePressed     (const plug::MousePressedEvent &event, plug::EHC &ehc)
{
    // plug::Transform tr (layout_->getPosition ());
    // plug::Transform unite = tr.combine (ehc.stack.top ());
    // plug::Vec2d pos_ = unite.apply (event.pos);
    if (close_button->covers (ehc.stack, event.pos))
    {
        close_button->onMousePressed (event, ehc);
        return;
    }
    if (covers (ehc.stack, event.pos))
    {
        is_moving_ = true;
        move_start_ = event.pos;
        ehc.stopped = true;
    }
}

void Header::onMouseReleased    (const plug::MouseReleasedEvent &event, plug::EHC &ehc)
{
    if (close_button->covers (ehc.stack, event.pos))
    {
        close_button->onMouseReleased (event, ehc);
        return;
    }
    if (is_moving_)
    {
        is_moving_ = false;
    }
}   

void Header::onMouseMove        (const plug::MouseMoveEvent &event, plug::EHC &ehc)
{
    if (close_button->covers (ehc.stack, event.pos))
    {
        close_button->onMouseMove (event, ehc);
    }
    if (is_moving_)
    {
        plug::Vec2d move = event.pos - move_start_;
        // parent_window_->get_transform ().offset_ += move;
        plug::LayoutBox &layout = parent_window_->getLayoutBox ();
        layout.setPosition (layout.getPosition () + move);
        parent_window_->setLayoutBox (layout);
        move_start_ = event.pos;

        ehc.stopped = true;
    }
}

void Header::onKeyboardPressed  (const plug::KeyboardPressedEvent &event, plug::EHC &ehc)
{
    return;
}

void Header::onKeyboardReleased (const plug::KeyboardReleasedEvent &event, plug::EHC &ehc)
{
    return;
}

void Header::onTick             (const plug::TickEvent &event, plug::EHC &ehc)
{
    return;
}

bool close_window (void *widget, void *arg)
{
    Window *window = (Window *)widget;
    if (window)
    {
        delete window;
        window = nullptr;
        return true;
    }
    return false;
}

bool Header::closeWindow () 
{
    return close_button ? close_button->run () : false;
};
