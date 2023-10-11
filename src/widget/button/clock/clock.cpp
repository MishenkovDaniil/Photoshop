#include "clock.h"
#include <stdlib.h>
#include <string.h>
#include <cstring>

Clock::Clock (Vector lh_corner, int width, int height, Button_run_fn func, Window *controlled_window, float hours, float minutes, float seconds, void *arg, Color fill_color, int run_mask) :
    Button (lh_corner, width, height, func, controlled_window, arg, fill_color ,run_mask),
    hours_ (hours),
    minutes_ (minutes), 
    seconds_ (seconds) 
{};

Clock::~Clock () {};

void Clock::render (sf::RenderTarget &target)
{
    Button::render (target);

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
    text.setPosition (lh_corner_.get_x () + (width_ - text_width) / 2, lh_corner_.get_y () + height_ / 2 - CLOCK_CHARACTER_SIZE / 2);
    
    // target.draw (button);
    target.draw (text);
}   

bool Clock::on_mouse_pressed  (Mouse_key mouse_key, Vector &pos)
{
    return false;
} 

bool Clock::on_mouse_released (Mouse_key mouse_key, Vector &pos)
{
    return false;
} 

bool Clock::on_mouse_moved    (Vector &new_pos)
{
    return false;
}   

bool Clock::on_keyboard_pressed  (Keyboard_key key)
{
    return false;
}   

bool Clock::on_keyboard_released (Keyboard_key key)
{
    return false;
}   

bool Clock::on_time (float delta_sec)
{
    seconds_ += delta_sec;
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

    return true;
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
