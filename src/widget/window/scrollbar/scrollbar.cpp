#include "scrollbar.h"

Scrollbar::Scrollbar (Vector rh_pos, int height, int obj_height, int obj_allowed_height, Window *window) :
    Button (rh_pos + Vector (-SCROLLBAR_WIDTH, SCROLLBAR_BUTTON_H), 
                         SCROLLBAR_WIDTH -2 , 
                         height - 2 * SCROLLBAR_BUTTON_H, 
                         change_canvas_rect_space, 
                         window, 
                         nullptr, 
                         Color (150, 150, 150)),
    scrollbar_height_ (height)
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
                         Black,
                         MOVE_BUTTON);
    
    buttons.add (up_);
    buttons.add (down_);
    buttons.add (mid_);

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
    
    for (size_t i = 0; i < buttons.get_size (); ++i)
    {
        buttons[i]->render (target, transform_stack);
    } 

    transform_stack.pop ();
    return;
}

bool Scrollbar::on_mouse_pressed  (Mouse_key mouse_key, Vector &pos, M_vector<Transform> &transform_stack) 
{
    Transform top = transform_stack.get_size () > 0 ? transform_stack.top () : Transform (Vector (0, 0));
    Transform unite = transform_.unite (top);
    transform_stack.push (unite);

    for (size_t i = 0; i < buttons.get_size (); ++i)
    {
        if (buttons[i]->on_mouse_pressed (mouse_key, pos, transform_stack)) 
        {
            transform_stack.pop ();
            return true;
        }
    }

    transform_stack.pop ();

    return false;
}

bool Scrollbar::on_mouse_released (Mouse_key mouse_key, Vector &pos, M_vector<Transform> &transform_stack) 
{
    Transform top = transform_stack.get_size () > 0 ? transform_stack.top () : Transform (Vector (0, 0));
    Transform unite = transform_.unite (top);
    transform_stack.push (unite);

    for (size_t i = 0; i < buttons.get_size (); ++i)
    {
        if (buttons[i]->on_mouse_released (mouse_key, pos, transform_stack))
        {
            transform_stack.pop ();
            return true;
        }
    }

    transform_stack.pop ();

    return false;
}

bool Scrollbar::on_mouse_moved    (Vector &new_pos, M_vector<Transform> &transform_stack) 
{
    Transform top = transform_stack.get_size () > 0 ? transform_stack.top () : Transform (Vector (0, 0));
    Transform unite = transform_.unite (top);
    transform_stack.push (unite);

    Vector new_pos_ = unite.apply_transform (new_pos);
    Vector mid_pos = new_pos;

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

    Vector mid_offset = mid_->transform_.offset_;

    double new_y = new_pos_.get_y();
    double old_y = mid_->press_pos_.get_y () + mid_offset.get_y ();
    double delta = std::abs (new_y - old_y);

    if (new_y < old_y && delta > mid_offset.get_y()) 
    {
        mid_pos += Vector (0, delta - mid_offset.get_y());
    }
    else if (new_y > old_y && delta > (height_ - mid_offset.get_y () - mid_->height_))
    {
        mid_pos += Vector (0, -delta + height_ - mid_offset.get_y () - mid_->height_);
    }
    
    if (mid_->on_mouse_moved (mid_pos, transform_stack)) 
    {
        double arg = (double)(mid_->transform_.offset_.get_y () - mid_offset.get_y ()) / (((double)(height_ - mid_->height_))); 

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
    assert (window->canvas_);

    int top = 0;
    sf::IntRect &rect = window->canvas_->get_draw_rect ();

    switch (*((sf::Keyboard::Key *)arg))
    {
        case sf::Keyboard::Key::Up:
        {
            top = std::max (rect.top - UP_DOWN_CANVAS_CHANGE, 0); //not friend but canvas member func ()
            break;
        }
        case sf::Keyboard::Key::Down:
        {
            top = std::min ((unsigned int)(rect.top + UP_DOWN_CANVAS_CHANGE), window->canvas_->canvas_texture.getSize ().y - window->canvas_->height_);
            break;
        }
        default:
        {
            return false;
        }
    }

    window->canvas_->set_draw_rect_offset (rect.left, top);
    return true;
} 

bool change_canvas_rect_mid (void *window_, void *arg)
{
    Window *window = (Window *)window_;
    double val = *(double *)arg;

    int texture_height = window->canvas_->canvas_texture.getSize ().y;
    int real_height = window->canvas_->height_;

    sf::IntRect &rect = window->canvas_->get_draw_rect ();

    int top = rect.top;
    
    top += val * (double)(texture_height - real_height);
    top = std::min (top, texture_height - real_height);

    if (top < 0) top = 0;

    window->canvas_->set_draw_rect_offset (rect.left, top);

    return true;
}

bool change_canvas_rect_space (void *window, void *arg)
{
    return true;
}
