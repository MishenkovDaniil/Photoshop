#include "clock.h"
#include <stdlib.h>
#include <string.h>
#include <cstring>

Clock::Clock (Vec2d lh_corner, int width, int height, Button_run_fn func, Window *controlled_window, float hours, float minutes, float seconds, void *arg, Color fill_color, int run_mask) :
    Button (lh_corner, width, height, func, controlled_window, arg, fill_color ,run_mask),
    hours_ (hours),
    minutes_ (minutes), 
    seconds_ (seconds) 
{};

void Clock::render (sf::RenderTarget &target, TransformStack &transform_stack)
{
    Button::render (target, transform_stack);
    
    Transform tr (layout_->get_position ());
    Transform unite = tr.combine (transform_stack.top ());
    Vec2d lh_pos = unite.getOffset ();

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

void Clock::onMousePressed     (MousePressedEvent &event, EHC &ehc)
{
    printf ("clock\n");
    return;
}

void Clock::onMouseReleased    (MouseReleasedEvent &event, EHC &ehc)
{
    return;
}

void Clock::onMouseMove        (MouseMoveEvent &event, EHC &ehc)
{
    return;
}

void Clock::onKeyboardPressed  (KeyboardPressedEvent &event, EHC &ehc)
{
    return;
}

void Clock::onKeyboardReleased (KeyboardReleasedEvent &event, EHC &ehc)
{
    return;
}

void Clock::onTick             (TickEvent &event, EHC &ehc)
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


// bool Clock::on_mouse_pressed  (MouseButton mouse_button, Vec2d &pos, TransformStack &transform_stack)
// {
//     return false;
// } 

// bool Clock::on_mouse_released (MouseButton mouse_button, Vec2d &pos, TransformStack &transform_stack)
// {
//     return false;
// } 

// bool Clock::on_mouse_moved    (Vec2d &new_pos, TransformStack &transform_stack)
// {
//     return false;
// }   

// bool Clock::on_keyboard_pressed  (KeyCode key)
// {
//     return false;
// }   

// bool Clock::on_keyboard_released (KeyCode key)
// {
//     return false;
// }   

// bool Clock::on_tick (float delta_sec)
// {
//     seconds_ += delta_sec;
//     if ((int)seconds_ > 59)
//     {
//         seconds_ = (int)seconds_ % 60;
//         minutes_++;

//         if ((int)minutes_ > 59)
//         {
//             minutes_ = (int)minutes_ % 60;
//             hours_++;

//             if ((int)hours_ > 23)
//             {
//                hours_ = (int)hours_ % 24;
//             }
//         }
//     }

//     return true;
// }   

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
