#include "clock.h"
#include <stdlib.h>
#include <string.h>
#include <cstring>

#include "../../../graphics/rendertexture/rendertexture.h"
#include "../../../graphics/rectangleshape/rectangleshape.h"
#include "../../../graphics/text/text.h"
#include "../../../graphics/font/font.h"
#include "../../../graphic_structures/color/color.h"

Clock::Clock (plug::Vec2d lh_corner, int width, int height, Button_run_fn func, Window *controlled_window, float hours, float minutes, float seconds, void *arg, plug::Color fill_color, int run_mask) :
    Button (lh_corner, width, height, func, controlled_window, arg, fill_color ,run_mask),
    hours_ (hours),
    minutes_ (minutes), 
    seconds_ (seconds) 
{};

void Clock::draw (plug::TransformStack &transform_stack, plug::RenderTarget &target)
{
    Button::draw (transform_stack, target);
    
    plug::Transform tr (layout_->getPosition ());
    plug::Transform unite = tr.combine (transform_stack.top ());
    plug::Vec2d lh_pos = unite.getOffset ();

    sprintf (hrs, "%d", (int)hours_);
    sprintf (min, "%d", (int)minutes_);
    sprintf (sec, "%d", (int)seconds_);
    sprintf (time_string, "%s:%s:%s", hrs, min, sec);
    
    Text text;
    Font font;
    font.loadFromFile (FONT_PATH);
    text.setString (time_string);
    text.setFont (font);
    text.setFillColor (plug::White);
    text.setCharacterSize (CLOCK_CHARACTER_SIZE);

    double text_width = text.findCharacterPos(8).x - text.findCharacterPos (0).x;
    text.setPosition (lh_pos.get_x () + (width_ - text_width) / 2, lh_pos.get_y () + height_ / 2 - CLOCK_CHARACTER_SIZE / 2);
    
    // target.draw (button);
    ((RenderTexture &)target).draw (text);
}   

void Clock::onMousePressed     (const plug::MousePressedEvent &event, plug::EHC &ehc)
{
    return;
}

void Clock::onMouseReleased    (const plug::MouseReleasedEvent &event, plug::EHC &ehc)
{
    return;
}

void Clock::onMouseMove        (const plug::MouseMoveEvent &event, plug::EHC &ehc)
{
    return;
}

void Clock::onKeyboardPressed  (const plug::KeyboardPressedEvent &event, plug::EHC &ehc)
{
    return;
}

void Clock::onKeyboardReleased (const plug::KeyboardReleasedEvent &event, plug::EHC &ehc)
{
    return;
}

void Clock::onTick             (const plug::TickEvent &event, plug::EHC &ehc)
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

void Clock::change_time (float seconds, float minutes, float hours)
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
