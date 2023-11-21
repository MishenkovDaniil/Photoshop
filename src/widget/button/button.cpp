#include <iostream>
#include <cassert>
#include <SFML/Graphics.hpp>

#include "button.h"

// may be made base class for text and image to add it as parameter in button and use int it whether image or text
//image of our size-->texture-->sprite-->sprite.set_texture () and sprite.set_texture_rect ()

plug::Button::Button (Vec2d lh_corner, int width, int height, Button_run_fn func, void *controlled_widget, void *arg, plug::Color fill_color, int run_mask) :
            width_ (width),
            height_ (height),
            run_fn_ (func),
            fill_color_ (fill_color),
            controlled_widget_ (controlled_widget),
            run_mask_ (run_mask),
            arg_ (arg), 
            press_pos_ (plug::Vec2d ()) 
{
    layout_ = new plug::Default_layout_box (lh_corner, plug::Vec2d (width, height));
    assert (layout_);
};

bool plug::Button::contains (double x, double y) const
{
    plug::Vec2d size = layout_->get_size ();

    return (0 <= x && 0 <= y &&
            x <= size.get_x () && y <= size.get_y ());
}

bool plug::Button::run ()
{
    if (!run_fn_)
    {
        fprintf (stderr, "Warning: nullptr function pointer in Button.\n");
        return false;
    }
    
    return run_fn_ (controlled_widget_, arg_);
}

void plug::Button::render (sf::RenderTarget &target, plug::TransformStack &transform_stack)
{
    plug::Transform tr (layout_->get_position ());
    plug::Transform unite = tr.combine (transform_stack.top ());
    plug::Vec2d lh_corner = unite.getOffset ();
    plug::Vec2d size = layout_->get_size ();

    // sf::RectangleShape rect (unite.scale_apply(size));
    sf::RectangleShape rect (size);
    rect.setFillColor (fill_color_);
/*TODO: 
//make constant for thickness and add possibility to change it (in constructor for example)*/
    rect.setOutlineThickness (1); 
    rect.setPosition (lh_corner.get_x (), lh_corner.get_y ());

    target.draw (rect);
}   


void plug::Button::onMousePressed     (const plug::MousePressedEvent &event, plug::EHC &ehc)
{
    plug::Transform tr (layout_->get_position ());
    plug::Transform unite = tr.combine (ehc.stack.top ());
    plug::Vec2d pos_ = unite.apply (event.pos);

    if (contains (pos_.get_x (), pos_.get_y ()))
    {
        if (run_mask_ & PRESS_BUTTON)
        {
            ehc.stopped = run ();
            return;
        }
        press_pos_ = pos_;
        is_pressed_ = true;
        ehc.stopped = true;
        return;
    }

    // return false;
} 

void plug::Button::onMouseReleased    (const plug::MouseReleasedEvent &event, plug::EHC &ehc)
{
    plug::Transform tr (layout_->get_position ());
    plug::Transform unite = tr.combine (ehc.stack.top ());
    plug::Vec2d pos_ = unite.apply (event.pos);

    if (contains (pos_.get_x (), pos_.get_y ()))
    {
        if (run_mask_ & RELEASE_BUTTON)
        {
            ehc.stopped = run ();
            return;
        }
    }

    if (is_pressed_)
    {
        is_pressed_ = false;
        ehc.stopped = true;
        return;
    }

    // return false;
} 

void plug::Button::onMouseMove        (const plug::MouseMoveEvent &event, plug::EHC &ehc)
{
    plug::Transform tr (layout_->get_position ());
    plug::Transform unite = tr.combine (ehc.stack.top ());
    plug::Vec2d pos_ = unite.apply (event.pos);
    
    if ((run_mask_ & MOVE_BUTTON) && is_pressed_)
    {
        Vec2d change (0, pos_.get_y () - press_pos_.get_y ());
        layout_->set_position (layout_->get_position () + change);

        ehc.stopped = true;
        return;
    }

    // return false;
} 

void plug::Button::onKeyboardPressed  (const plug::KeyboardPressedEvent &event, plug::EHC &ehc)
{
    return;
} 

void plug::Button::onKeyboardReleased (const plug::KeyboardReleasedEvent &event, plug::EHC &ehc)
{
    return;
} 

void plug::Button::onTick             (const plug::TickEvent &event, plug::EHC &ehc)
{
    return;
} 


plug::Texture_button::Texture_button (Vec2d lh_corner, int width, int height, sf::Texture &pressed, sf::Texture &released, 
                                Button_run_fn func, void *controlled_widget, void *arg, int run_mask) : 
    plug::Button (lh_corner, width, height, func, controlled_widget, arg, plug::Color (), run_mask),
    pressed_texture_ (pressed),
    released_texture_ (released) 
{
    cur_texture_ = &released_texture_;
    sprite_ = new sf::Sprite;
}

plug::Texture_button::~Texture_button ()
{
    if (sprite_)
        delete sprite_;
}

void plug::Texture_button::render (sf::RenderTarget &target, TransformStack &transform_stack)
{
    plug::Transform tr (layout_->get_position ());
    plug::Transform unite = tr.combine (transform_stack.top ());
    plug::Vec2d lh_corner = unite.getOffset ();

    sprite_->setTexture (*cur_texture_);
    sprite_->setPosition (lh_corner.get_x (), lh_corner.get_y ());

    target.draw (*sprite_);
}

void plug::Texture_button::onMousePressed     (const plug::MousePressedEvent &event, plug::EHC &ehc)
{
    plug::Button::onMousePressed (event, ehc);
    bool status = ehc.stopped;
    cur_texture_ = is_pressed_ ? &pressed_texture_ : &released_texture_;
}

void plug::Texture_button::onMouseReleased    (const plug::MouseReleasedEvent &event, plug::EHC &ehc)
{
    plug::Button::onMouseReleased (event, ehc);
    cur_texture_ = is_pressed_ ? &pressed_texture_ : &released_texture_;
}

void plug::Texture_button::onMouseMove        (const plug::MouseMoveEvent &event, plug::EHC &ehc)
{
    plug::Button::onMouseMove (event, ehc);
    cur_texture_ = is_pressed_ ? &pressed_texture_ : &released_texture_;
}

void plug::Texture_button::onKeyboardPressed  (const plug::KeyboardPressedEvent &event, plug::EHC &ehc)
{
    plug::Button::onKeyboardPressed (event, ehc);
    cur_texture_ = is_pressed_ ? &pressed_texture_ : &released_texture_;
}

void plug::Texture_button::onKeyboardReleased (const plug::KeyboardReleasedEvent &event, plug::EHC &ehc)
{
    plug::Button::onKeyboardReleased (event, ehc);
    cur_texture_ = is_pressed_ ? &pressed_texture_ : &released_texture_;
}


plug::String_button::String_button (Vec2d lh_corner, int width, int height, const char *string, const Color &pressed_color, const Color &released_color, Button_run_fn func, void *controlled_widget,
               void *arg, int run_mask) :
    plug::Button (lh_corner, width, height, func, controlled_widget, arg, Transparent, run_mask),
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

plug::String_button::~String_button () 
{
    if (string_) delete[] string_;
}

void plug::String_button::render (sf::RenderTarget &target, plug::TransformStack &transform_stack)
{
    assert (layout_);

    transform_stack.enter (plug::Transform (layout_->get_position ()));
    plug::Vec2d lh_pos = transform_stack.top ().getOffset ();

    plug::Vec2d size = layout_->get_size ();
    plug::Vec2d real_size = size;
    // transform_stack.top ().scale_apply (size);
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

    transform_stack.leave ();
}


void plug::String_button::onMousePressed     (const plug::MousePressedEvent &event, plug::EHC &ehc)
{
    plug::Button::onMousePressed (event, ehc);
    cur_color_ = is_pressed_ ? &pressed_color_ : &released_color_;
}

void plug::String_button::onMouseReleased    (const plug::MouseReleasedEvent &event, plug::EHC &ehc)
{
    plug::Button::onMouseReleased (event, ehc);
    cur_color_ = is_pressed_ ? &pressed_color_ : &released_color_;
}

void plug::String_button::onMouseMove        (const plug::MouseMoveEvent &event, plug::EHC &ehc)
{
    plug::Button::onMouseMove (event, ehc);
    cur_color_ = is_pressed_ ? &pressed_color_ : &released_color_;
}

void plug::String_button::onKeyboardPressed  (const plug::KeyboardPressedEvent &event, plug::EHC &ehc)
{
    plug::Button::onKeyboardPressed (event, ehc);
    cur_color_ = is_pressed_ ? &pressed_color_ : &released_color_;
}

void plug::String_button::onKeyboardReleased (const plug::KeyboardReleasedEvent &event, plug::EHC &ehc)
{
    plug::Button::onKeyboardReleased (event, ehc);
    cur_color_ = is_pressed_ ? &pressed_color_ : &released_color_;
}



plug::List_button::List_button (plug::Button *list_button) :
    list_button_ (list_button)
{
    assert (list_button);
    layout_ = new plug::Default_layout_box (plug::Vec2d (list_button->layout_->get_position () + plug::Vec2d (0, list_button->layout_->get_size ().get_y ())), 
                                            list_button->layout_->get_size ());
    assert (layout_);

}

plug::List_button::~List_button ()
{
    if (layout_) delete layout_;
    layout_ = nullptr;
}

void plug::List_button::add_button (plug::Button *button)
{
    assert (button);
    plug::Layout_box *old_layout = button->layout_;
    assert (old_layout);

    size_t button_height = old_layout->get_size ().get_y ();
    delete old_layout;
    
    plug::Default_layout_box *new_layout = new plug::Default_layout_box (plug::Vec2d (0, relative_height_), 
                                                                         plug::Vec2d (layout_->get_size ().get_x (), button_height));
    relative_height_ += button_height;
    button->set_layout_box (*new_layout);
    
    buttons_.add (button);
}


void plug::List_button::render (sf::RenderTarget &target, plug::TransformStack &transform_stack)
{
    list_button_->render (target, transform_stack);

    if (is_open_)
    {
        transform_stack.enter (plug::Transform (layout_->get_position ()));

        size_t button_num = buttons_.get_size ();
        for (int button_idx = 0; button_idx < button_num; ++button_idx)
        {
            plug::Button *button = buttons_[button_idx];
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

        transform_stack.leave ();
    }
}   

void plug::List_button::onMousePressed     (const plug::MousePressedEvent &event, plug::EHC &ehc) // rm status
{
    assert (layout_);
    
    bool status = false;

    plug::Transform list_button_tr (list_button_->layout_->get_position ());
    plug::Transform unite = list_button_tr.combine (ehc.stack.top ());
    plug::Vec2d pos_ = unite.apply (event.pos);

    list_button_->onMousePressed (event, ehc);
    status |= ehc.stopped;

    if (list_button_->contains (pos_.get_x (), pos_.get_y ()))
    {
        is_open_ = !is_open_;
        ehc.stopped = true;
        return;
    }
    
    if (!is_open_)
    {
        return;
    }

    ehc.stack.enter (plug::Transform (layout_->get_position ()));
    
    size_t button_num = buttons_.get_size ();

    for (int button_idx = 0; button_idx < button_num; ++button_idx)
    {
        plug::Button *button = buttons_[button_idx];
        if (!button)
        {
            fprintf (stderr, "Event error: nil button is detected.\n"
                            "Hint: nil button idx in buttons vector = %d.\n"
                            "Hint: buttons vector size = %lu.\n", button_idx, button_num);
            ehc.stopped = true;
            return;
        }
        assert (button);

        button->onMousePressed (event, ehc);
        status |= ehc.stopped;
    }

    ehc.stack.leave ();
    
    if ((!status))
    {
        is_open_ = !is_open_;
        status = true;
    }

    ehc.stopped = status;
    // return status;
}

void plug::List_button::onMouseReleased    (const plug::MouseReleasedEvent &event, plug::EHC &ehc)
{
    bool status = false;
    
    list_button_->onMouseReleased (event, ehc);
    status |= ehc.stopped;

    if (is_open_)
    {
        ehc.stack.enter (Transform (layout_->get_position ()));

        plug::Vec2d lh_pos = ehc.stack.top ().getOffset ();

        size_t button_num = buttons_.get_size ();
        for (int button_idx = 0; button_idx < button_num; ++button_idx)
        {
            plug::Button *button = buttons_[button_idx];
            if (!button)
            {
                fprintf (stderr, "Event error: nil button is detected.\n"
                                "Hint: nil button idx in buttons vector = %d.\n"
                                "Hint: buttons vector size = %lu.\n", button_idx, button_num);
                ehc.stopped = true;
                return;
            }
            assert (button);

            // event.pos = lh_pos; ///
            button->onMouseReleased (event, ehc);
            status |= ehc.stopped;
        }

        ehc.stack.leave ();
    }

    ehc.stopped = status;
    // return status;
}

void plug::List_button::onMouseMove        (const plug::MouseMoveEvent &event, plug::EHC &ehc)
{
    bool status = false;
    
    list_button_->onMouseMove (event, ehc);
    status |= ehc.stopped;
    
    if (is_open_)
    {
        ehc.stack.enter (plug::Transform (layout_->get_position ()));

        plug::Vec2d lh_pos = ehc.stack.top ().getOffset ();

        size_t button_num = buttons_.get_size ();
        for (int button_idx = 0; button_idx < button_num; ++button_idx)
        {
            plug::Button *button = buttons_[button_idx];
            if (!button)
            {
                fprintf (stderr, "Event error: nil button is detected.\n"
                                "Hint: nil button idx in buttons vector = %d.\n"
                                "Hint: buttons vector size = %lu.\n", button_idx, button_num);
                ehc.stopped = true;
                return;
            }
            assert (button);

            // event.pos = lh_pos; ///
            button->onMouseMove (event, ehc);
            status |= ehc.stopped;
        }

        ehc.stack.leave ();
    }

    ehc.stopped = status;
    // return status;
}

void plug::List_button::onKeyboardPressed  (const plug::KeyboardPressedEvent &event, plug::EHC &ehc)
{
    return;
}

void plug::List_button::onKeyboardReleased (const plug::KeyboardReleasedEvent &event, plug::EHC &ehc)
{
    return;
}

void plug::List_button::onTick             (const plug::TickEvent &event, plug::EHC &ehc)
{
    return;
}

