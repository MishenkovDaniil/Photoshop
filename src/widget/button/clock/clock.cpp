#include "clock.h"
#include <stdlib.h>
#include <string.h>
#include <cstring>

plug::Clock::Clock (plug::Vec2d lh_corner, int width, int height, Button_run_fn func, plug::Window *controlled_window, float hours, float minutes, float seconds, void *arg, plug::Color fill_color, int run_mask) :
    plug::Button (lh_corner, width, height, func, controlled_window, arg, fill_color ,run_mask),
    hours_ (hours),
    minutes_ (minutes), 
    seconds_ (seconds) 
{};

void plug::Clock::render (sf::RenderTarget &target, plug::TransformStack &transform_stack)
{
    plug::Button::render (target, transform_stack);
    
    plug::Transform tr (layout_->get_position ());
    plug::Transform unite = tr.combine (transform_stack.top ());
    plug::Vec2d lh_pos = unite.getOffset ();

    sprintf (hrs, "%d", (int)hours_);
    sprintf (min, "%d", (int)minutes_);
    sprintf (sec, "%d", (int)seconds_);
    sprintf (clck, "%s:%s:%s", hrs, min, sec);
    
    sf::Text text;
    sf::Font font;
    font.loadFromFile (font_path);
    text.setString (clck);
    text.setFont (font);
    text.setFillColor (sf::Color::White);
    text.setCharacterSize (CLOCK_CHARACTER_SIZE);

    double text_width = text.findCharacterPos(8).x - text.findCharacterPos (0).x;
    text.setPosition (lh_pos.get_x () + (width_ - text_width) / 2, lh_pos.get_y () + height_ / 2 - CLOCK_CHARACTER_SIZE / 2);
    
    // target.draw (button);
    target.draw (text);
}   

void plug::Clock::onMousePressed     (const plug::MousePressedEvent &event, plug::EHC &ehc)
{
    return;
}

void plug::Clock::onMouseReleased    (const plug::MouseReleasedEvent &event, plug::EHC &ehc)
{
    return;
}

void plug::Clock::onMouseMove        (const plug::MouseMoveEvent &event, plug::EHC &ehc)
{
    return;
}

void plug::Clock::onKeyboardPressed  (const plug::KeyboardPressedEvent &event, plug::EHC &ehc)
{
    return;
}

void plug::Clock::onKeyboardReleased (const plug::KeyboardReleasedEvent &event, plug::EHC &ehc)
{
    return;
}

void plug::Clock::onTick             (const plug::TickEvent &event, plug::EHC &ehc)
{
    seconds_ += event.delta_time;
    if ((int)seconds_ > 59)
    {
        seconds_ = (int)seconds_ % 60;
        minutes_++;

        if ((int)minutes_ > 59)
        {
            minutes_ = (int)minutes_ % 60;
            hours_++;

            if ((int)hours_ > 23)
            {
               hours_ = (int)hours_ % 24;
            }
        }
    }

    ehc.stopped = true;
    // return true;
}

void plug::Clock::change_time (float seconds, float minutes, float hours)
{   
    if (seconds < 0 || (int)seconds > 59)
    {
        fprintf (stderr, "Clock_error: seconds value in change time func is under zero.\n");
    }

    seconds_ = seconds;
    if (minutes > 0 && (int)minutes < 60)
    {
        minutes_ = minutes;
    }

    if (hours > 0 && (int)hours < 24)
    {
        hours_ = hours;
    }
}
