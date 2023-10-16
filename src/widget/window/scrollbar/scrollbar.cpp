#include "scrollbar.h"

Scrollbar::Scrollbar (Vector rh_pos, int height, int obj_height, int obj_allowed_height, Window *window) :
    Button (rh_pos + Vector (-SCROLLBAR_WIDTH, SCROLLBAR_BUTTON_H), 
                         SCROLLBAR_WIDTH -2 , 
                         height - 2 * SCROLLBAR_BUTTON_H, 
                         change_canvas_rect_space, 
                         window, 
                         nullptr, 
                         Color (150, 150, 150)),
    // lh_pos_ (rh_pos - Vector (SCROLLBAR_WIDTH, 0)),
    scrollbar_height_ (height),
    obj_height_ (obj_height),
    obj_allowed_height_ (obj_allowed_height)
{
    up_    = new Button (Vector (0, -SCROLLBAR_BUTTON_H), 
                         SCROLLBAR_WIDTH - 2, 
                         SCROLLBAR_BUTTON_H, 
                         change_canvas_rect_up, 
                         window,
                         nullptr, 
                         Color (50, 50, 50));
    down_  = new Button (Vector (0, height - 2 * SCROLLBAR_BUTTON_H),
                         SCROLLBAR_WIDTH - 2, 
                         SCROLLBAR_BUTTON_H, 
                         change_canvas_rect_down, 
                         window, 
                         nullptr, 
                         Color (50, 50, 50));
    mid_   = new Button (Vector (0, 0), 
                         SCROLLBAR_WIDTH - 2, 
                         100, 
                         change_canvas_rect_mid,
                         window,
                         nullptr, 
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


void Scrollbar::render (sf::RenderTarget &target, M_vector<Transform> &transform_stack) 
{
    Button::render  (target, transform_stack);

    Transform top = transform_stack.get_size () > 0 ? transform_stack.top () : Transform (Vector (0, 0));
    Transform unite = transform_.unite (top);
    transform_stack.push (unite);
    
     down_->render  (target, transform_stack);
      mid_->render  (target, transform_stack);
       up_->render  (target, transform_stack);

    transform_stack.pop ();
    return;
}

bool Scrollbar::on_mouse_pressed  (Mouse_key mouse_key, Vector &pos, M_vector<Transform> &transform_stack) 
{
    Transform top = transform_stack.get_size () > 0 ? transform_stack.top () : Transform (Vector (0, 0));
    Transform unite = transform_.unite (top);
    transform_stack.push (unite);

    if (   up_->on_mouse_pressed (mouse_key, pos, transform_stack))
    {   
        transform_stack.pop ();
        return true;
    }
    if ( down_->on_mouse_pressed (mouse_key, pos, transform_stack))
    {   
        transform_stack.pop ();
        return true;
    }
    if (  mid_->on_mouse_pressed (mouse_key, pos, transform_stack))
    {   
        transform_stack.pop ();
        return true;
    }

    transform_stack.pop ();

    return false;
}

bool Scrollbar::on_mouse_released (Mouse_key mouse_key, Vector &pos, M_vector<Transform> &transform_stack) 
{
    Transform top = transform_stack.get_size () > 0 ? transform_stack.top () : Transform (Vector (0, 0));
    Transform unite = transform_.unite (top);
    transform_stack.push (unite);

    if (   up_->on_mouse_released (mouse_key, pos, transform_stack)) 
    {   
        transform_stack.pop ();
        return true;
    }
    if ( down_->on_mouse_released (mouse_key, pos, transform_stack))
    {   
        transform_stack.pop ();
        return true;
    }
    if (  mid_->on_mouse_released (mouse_key, pos, transform_stack))
    {   
        transform_stack.pop ();
        return true;
    }

    transform_stack.pop ();

    return false;
}

bool Scrollbar::on_mouse_moved    (Vector &new_pos, M_vector<Transform> &transform_stack) 
{
    Transform top = transform_stack.get_size () > 0 ? transform_stack.top () : Transform (Vector (0, 0));
    Transform unite = transform_.unite (top);
    transform_stack.push (unite);

    if (   up_->on_mouse_moved (new_pos, transform_stack)) 
    {   
        transform_stack.pop ();
        return true;
    }
    if ( down_->on_mouse_moved (new_pos, transform_stack))
    {   
        transform_stack.pop ();
        return true;
    }

    Vector mid_lh_pos = mid_->transform_.offset_;

    double old_mid_y = mid_lh_pos.get_y ();
                                                    /////
    double new_y = new_pos.get_y();
    double old_y = mid_->press_pos_.get_y ();
    double delta = std::abs (new_y - old_y);

    if (new_y < old_y  &&  delta > mid_lh_pos.get_y () - mid_lh_pos.get_y ()) 
    {
        new_pos += Vector (0, delta + mid_lh_pos.get_y () - mid_lh_pos.get_y ());
    }
    else if (new_y > old_y && delta > (mid_lh_pos.get_y () + height_ - (mid_lh_pos.get_y () + mid_->height_)))
    {
        new_pos += Vector (0, -new_y + old_y + mid_lh_pos.get_y () + height_ - (mid_lh_pos.get_y () + mid_->height_));
    }

    if (mid_->on_mouse_moved (new_pos, transform_stack)) 
    {
        double arg = (double)(mid_lh_pos.get_y () - old_mid_y) / (((double)(height_ - mid_->height_))); 
        mid_->set_arg ((void *)&arg);
        mid_->run ();
        transform_stack.pop ();
        return true;
    }

    transform_stack.pop ();

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

bool change_canvas_rect_up (void *window, void *arg)
{
    sf::Keyboard::Key key = sf::Keyboard::Key::Up;

    return change_canvas_rect_up_down (window, &key);
}

bool change_canvas_rect_down (void *window, void *arg)
{
    sf::Keyboard::Key key = sf::Keyboard::Key::Down;

    return change_canvas_rect_up_down (window, &key);
}

bool change_canvas_rect_up_down (void *window_, void *arg)
{
    assert (window_);
    Window *window = (Window *)window_;

    switch (*((sf::Keyboard::Key *)arg))
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

bool change_canvas_rect_mid (void *window_, void *arg)
{
    Window *window = (Window *)window_;
    double val = *(double *)arg;

    int texture_height = window->canvas_->canvas_texture.getSize ().y;
    int real_height = window->canvas_->height_;

    int top = window->canvas_->draw_rect_.top;
    
    top += val * (double)(texture_height - real_height);
    top = std::min (top, texture_height - real_height);

    window->canvas_->draw_rect_.top = top; 

    return true;
}

bool change_canvas_rect_space (void *window, void *arg)
{
    return true;
}
