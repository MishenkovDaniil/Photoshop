#include "scrollbar.h"

Scrollbar::Scrollbar (Vector rh_pos, int height, int obj_height, int obj_allowed_height, Window *window) :
    Button (rh_pos + Vector (-SCROLLBAR_WIDTH, SCROLLBAR_BUTTON_H), 
                         SCROLLBAR_WIDTH -2 , 
                         height - 2 * SCROLLBAR_BUTTON_H, 
                         change_canvas_rect_space, 
                         window, 
                         Color (150, 150, 150)),
    lh_pos_ (rh_pos - Vector (SCROLLBAR_WIDTH, 0)),
    scrollbar_height_ (height),
    obj_height_ (obj_height),
    obj_allowed_height_ (obj_allowed_height)
{
    up_    = new Button (lh_pos_, 
                         SCROLLBAR_WIDTH - 2, 
                         SCROLLBAR_BUTTON_H, 
                         change_canvas_rect_up, 
                         window, 
                         Color (50, 50, 50));
    down_  = new Button (lh_pos_ + Vector (0, height - SCROLLBAR_BUTTON_H),
                         SCROLLBAR_WIDTH - 2, 
                         SCROLLBAR_BUTTON_H, 
                         change_canvas_rect_down, 
                         window, 
                         Color (50, 50, 50));
    mid_   = new Button (lh_pos_ + Vector (0, height / 2 - 50), 
                         SCROLLBAR_WIDTH - 2, 
                         100, 
                         change_canvas_rect_mid,
                         window,
                         Color (0, 0, 0),
                         MOVE_BUTTON);

    assert (up_ && down_ && mid_ && "failed to allocate scrollbar");
}

Scrollbar::~Scrollbar ()
{
    delete mid_;    
    delete down_;    
    delete up_;    
};


void Scrollbar::render (sf::RenderTarget &target) 
{
    Button::render  (target);
     down_->render  (target);
      mid_->render  (target);
       up_->render  (target);

    return;
}

bool Scrollbar::on_mouse_pressed  (Mouse_key mouse_key, Vector &pos) 
{
    if (   up_->on_mouse_pressed (mouse_key, pos)) return true;
    if ( down_->on_mouse_pressed (mouse_key, pos)) return true;
    if (  mid_->on_mouse_pressed (mouse_key, pos)) return true;

    return false;
}

bool Scrollbar::on_mouse_released (Mouse_key mouse_key, Vector &pos) 
{
    if (   up_->on_mouse_released (mouse_key, pos)) return true;
    if ( down_->on_mouse_released (mouse_key, pos)) return true;
    if (  mid_->on_mouse_released (mouse_key, pos)) return true;

    return false;
}

bool Scrollbar::on_mouse_moved    (Vector &new_pos) 
{
    if (   up_->on_mouse_moved (new_pos)) return true;
    if ( down_->on_mouse_moved (new_pos)) return true;

    double new_y = new_pos.get_y();
    double old_y = mid_->press_pos_.get_y ();
    double delta = std::abs (new_y - old_y);

    if (new_y < old_y  &&  delta > mid_->lh_corner_.get_y () - lh_corner_.get_y ()) 
    {
        new_pos += Vector (0, delta + lh_corner_.get_y () - mid_->lh_corner_.get_y ());
    }
    else if (new_y > old_y && delta > (lh_corner_.get_y () + height_ - (mid_->lh_corner_.get_y () + mid_->height_)))
    {
        new_pos += Vector (0, -new_y + old_y + lh_corner_.get_y () + height_ - (mid_->lh_corner_.get_y () + mid_->height_));
    }

    if (mid_->on_mouse_moved (new_pos)) return true;

    return false;
}   

bool Scrollbar::on_keyboard_pressed  (Keyboard_key key) 
{
    //TODO....
    return false;
}

bool Scrollbar::on_keyboard_released (Keyboard_key key) 
{
    //TODO....
    return false;
}

bool Scrollbar::on_time (float delta_sec) 
{
    //TODO....
    return false;
}

bool change_canvas_rect_up (Window *window, sf::Keyboard::Key key)
{
    return change_canvas_rect_up_down (window, sf::Keyboard::Key::Up);
}

bool change_canvas_rect_down (Window *window, sf::Keyboard::Key key)
{
    return change_canvas_rect_up_down (window, sf::Keyboard::Key::Down);
}

bool change_canvas_rect_up_down (Window *window, sf::Keyboard::Key key)
{
    assert (window);
    switch (key)
    {
        case sf::Keyboard::Key::Up:
        {
            window->canvas_->draw_rect_.top = std::max (window->canvas_->draw_rect_.top - 10, 0);
            break;
        }
        case sf::Keyboard::Key::Down:
        {
            window->canvas_->draw_rect_.top = std::min ((unsigned int)(window->canvas_->draw_rect_.top + 10), window->canvas_->canvas_texture.getSize ().y - window->canvas_->height_);
            break;
        }
        default:
        {
            return false;
        }
    }
    return true;
} 

bool change_canvas_rect_mid (Window *window, sf::Keyboard::Key key)
{
    return true;
}

bool change_canvas_rect_space (Window *window, sf::Keyboard::Key key)
{
    return true;
}