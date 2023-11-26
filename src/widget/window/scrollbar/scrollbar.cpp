#include "scrollbar.h"

Scrollbar::Scrollbar (plug::Vec2d rh_pos, int height, int obj_height, int obj_allowed_height, Window *window) :
    Button (rh_pos + plug::Vec2d (-SCROLLBAR_WIDTH, SCROLLBAR_BUTTON_H), 
                         SCROLLBAR_WIDTH -2 , 
                         height - 2 * SCROLLBAR_BUTTON_H, 
                         change_canvas_rect_space, 
                         window, 
                         nullptr, 
                         plug::Color (150, 150, 150)),
    scrollbar_height_ (height)
{
    up_    = new Button (plug::Vec2d (0, -SCROLLBAR_BUTTON_H), 
                        SCROLLBAR_WIDTH - 2, 
                        SCROLLBAR_BUTTON_H, 
                        change_canvas_rect_up, 
                        window,
                        nullptr, 
                        plug::Color (50, 50, 50));

    down_  = new Button (plug::Vec2d (0, height - 2 * SCROLLBAR_BUTTON_H),
                        SCROLLBAR_WIDTH - 2, 
                        SCROLLBAR_BUTTON_H, 
                        change_canvas_rect_down, 
                        window, 
                        nullptr, 
                        plug::Color (50, 50, 50));

    mid_   = new Button (plug::Vec2d (0, 0), 
                        SCROLLBAR_WIDTH - 2, 
                        100, 
                        change_canvas_rect_mid,
                        window,
                        nullptr, 
                        plug::Black,
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


void Scrollbar::render (plug::RenderTarget &target, plug::TransformStack &transform_stack) 
{
    Button::render  (target, transform_stack);

    transform_stack.enter (plug::Transform (layout_->getPosition ()));

    for (size_t i = 0; i < buttons.get_size (); ++i)
    {
        buttons[i]->render (target, transform_stack);
    } 

    transform_stack.leave ();
    return;
}


void Scrollbar::onMousePressed     (const plug::MousePressedEvent &event, plug::EHC &ehc)
{
    ehc.stack.enter (plug::Transform (layout_->getPosition ()));

    for (size_t i = 0; i < buttons.get_size (); ++i)
    {
        buttons[i]->onMousePressed (event, ehc);
        if (ehc.stopped)
        {
            ehc.stack.leave ();
            return;
        }
    }

    ehc.stack.leave ();
}

void Scrollbar::onMouseReleased    (const plug::MouseReleasedEvent &event, plug::EHC &ehc)
{
    ehc.stack.enter (plug::Transform (layout_->getPosition ()));

    for (size_t i = 0; i < buttons.get_size (); ++i)
    {
        buttons[i]->onMouseReleased (event, ehc);
    }

    ehc.stack.leave ();
}

void Scrollbar::onMouseMove        (const plug::MouseMoveEvent &event, plug::EHC &ehc)
{
    ehc.stack.enter (plug::Transform (layout_->getPosition ()));
    int leave_num = 1;
    plug::Vec2d new_pos_ = ehc.stack.top ().apply (event.pos);
    plug::Vec2d mid_pos = event.pos;

    up_->onMouseMove (event, ehc);
    if (ehc.stopped) 
    {
        ehc.stack.leave ();
        return;
    }
    down_->onMouseMove (event, ehc);
    if (ehc.stopped)
    {
        ehc.stack.leave ();
        return;
    }

    // Vec2d mid_offset = mid_->transform_.offset_;
    plug::Vec2d mid_offset = mid_->layout_->getPosition ();

    double new_y = new_pos_.get_y();
    double old_y = mid_->press_pos_.get_y () + mid_offset.get_y (); 
    double delta = std::abs (new_y - old_y);

    if (new_y < old_y && delta > mid_offset.get_y()) 
    {
        mid_pos += plug::Vec2d (0, delta - mid_offset.get_y());
        ehc.stack.enter (plug::Transform (plug::Vec2d (0, -(delta - mid_offset.get_y()))));
        leave_num++;
    }
    else if (new_y > old_y && delta > (height_ - mid_offset.get_y () - mid_->height_))
    {
        mid_pos += plug::Vec2d (0, -delta + height_ - mid_offset.get_y () - mid_->height_);
        ehc.stack.enter (plug::Transform (plug::Vec2d (0, -(-delta + height_ - mid_offset.get_y () - mid_->height_))));
        leave_num++;
    }
    
    mid_->onMouseMove (event, ehc);
    if (ehc.stopped) 
    {
        // double arg = (double)(mid_->transform_.offset_.get_y () - mid_offset.get_y ()) / (((double)(height_ - mid_->height_))); 
        double arg = (double)(mid_->layout_->getPosition ().get_y () - mid_offset.get_y ()) / (((double)(height_ - mid_->height_))); 
        mid_->set_arg ((void *)&arg);
        mid_->run ();
    }

    while (leave_num--)
        ehc.stack.leave ();
}

void Scrollbar::onKeyboardPressed  (const plug::KeyboardPressedEvent &event, plug::EHC &ehc)
{
    return;
}

void Scrollbar::onKeyboardReleased (const plug::KeyboardReleasedEvent &event, plug::EHC &ehc)
{
    return;
}

void Scrollbar::onTick             (const plug::TickEvent &event, plug::EHC &ehc)
{
    return;
}


bool change_canvas_rect_up (void *window, void *arg)
{
    plug::KeyCode key = plug::KeyCode::Up;

    return change_canvas_rect_up_down (window, &key);
}

bool change_canvas_rect_down (void *window, void *arg)
{
    plug::KeyCode key = plug::KeyCode::Down;

    return change_canvas_rect_up_down (window, &key);
}

bool change_canvas_rect_up_down (void *window_, void *arg)
{
    assert (window_);
    Window *window = (Window *)window_;
    assert (window->canvas_);

    int top = 0;
    IntRect &rect = window->canvas_->getDrawRect ();

    switch (*((plug::KeyCode *)arg))
    {
        case plug::KeyCode::Up:
        {
            top = std::max (rect.getTopCorner () - UP_DOWN_CANVAS_CHANGE, 0); //not friend but canvas member func ()
            break;
        }
        case plug::KeyCode::Down:
        {
            top = std::min ((unsigned int)(rect.getTopCorner () + UP_DOWN_CANVAS_CHANGE), 
                            (unsigned int)(window->canvas_->getFullSize ().y - window->canvas_->getSize ().y));
            break;
        }
        default:
        {
            return false;
        }
    }

    window->canvas_->setDrawRectOffset (rect.getLeftCorner (), top);
    return true;
} 

bool change_canvas_rect_mid (void *window_, void *arg)
{
    Window *window = (Window *)window_;
    double val = *(double *)arg;
    static double good_top = 0;

    int texture_height = window->canvas_->getFullSize ().y;
    int real_height = window->canvas_->getSize ().y;

    IntRect &rect = window->canvas_->getDrawRect ();

    // int top = rect.top;
    if (rect.getTopCorner () != (int)good_top)
        good_top = rect.getTopCorner ();
    good_top += val * (double)(texture_height - real_height);
    // top = std::min (top, texture_height - real_height);
    if (good_top > texture_height - real_height)
    {
        good_top = texture_height - real_height;
    }
    if (good_top < 0) 
    {
        good_top = 0;
    }

    window->canvas_->setDrawRectOffset (rect.getLeftCorner (), good_top);

    return true;
}

bool change_canvas_rect_space (void *window, void *arg)
{
    return true;
}
