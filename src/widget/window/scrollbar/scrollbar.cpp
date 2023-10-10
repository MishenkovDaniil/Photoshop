#include "scrollbar.h"

Scrollbar::Scrollbar (Vector rh_pos, int height, int obj_height, int obj_allowed_height, Window *window) :
    lh_pos_ (rh_pos - Vector (SCROLLBAR_WIDTH, 0)),
    height_ (height),
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
                         nullptr,
                         nullptr);
    space_ = new Button (lh_pos_ + Vector (0, SCROLLBAR_BUTTON_H), 
                         SCROLLBAR_WIDTH -2 , 
                         height - 2 * SCROLLBAR_BUTTON_H, 
                         nullptr, 
                         nullptr, 
                         Color (150, 150, 150));
    assert (up_ && down_ && mid_ && space_ && "failed to allocate scrollbar");
}

Scrollbar::~Scrollbar ()
{
    delete space_;    
    delete mid_;    
    delete down_;    
    delete up_;    
};


void Scrollbar::render (sf::RenderTarget &target) 
{
    space_->render  (target);
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
    if (space_->on_mouse_pressed (mouse_key, pos)) return true;

    return false;
}

bool Scrollbar::on_mouse_released (Mouse_key mouse_key, Vector &pos) 
{
    printf ("up\n");
    if (   up_->on_mouse_released (mouse_key, pos)) return true;
    printf ("down\n");
    if ( down_->on_mouse_released (mouse_key, pos)) return true;
    printf ("mid_\n");
    if (  mid_->on_mouse_released (mouse_key, pos)) return true;
    printf ("space_\n");
    if (space_->on_mouse_released (mouse_key, pos)) return true;

    return false;
}

bool Scrollbar::on_mouse_moved    (Vector &new_pos) 
{
    if (   up_->on_mouse_moved (new_pos)) return true;
    if ( down_->on_mouse_moved (new_pos)) return true;
    if (  mid_->on_mouse_moved (new_pos)) return true;
    if (space_->on_mouse_moved (new_pos)) return true;

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
    printf ("up\n");
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
            window->canvas_->draw_rect_.top = std::min ((unsigned int)(window->canvas_->draw_rect_.top + 10), window->canvas_->canvas_texture.getSize ().y - window->canvas_->height_);
            break;
        }
        case sf::Keyboard::Key::Down:
        {
            window->canvas_->draw_rect_.top = std::max (window->canvas_->draw_rect_.top - 10, 0);
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
    ;
}

bool change_canvas_rect_space (Window *window, sf::Keyboard::Key key)
{
    ;
}
