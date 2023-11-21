#include "canvas.h"

bool plug::SelectionMask::get_pixel (size_t x, size_t y) const
{
    assert (mask && "nullptr filter mask\n");
    assert ((x < width_) && (y < height_) && "invalid pixel coord\n");

    return mask[x + y * width_];
}  

void plug::SelectionMask::set_pixel (size_t x, size_t y, bool flag)
{
    assert (mask && "nullptr filter mask\n");
    assert ((x < width_) && (y < height_) && "invalid pixel coord\n");

    mask[x + y * width_] = flag;
}  

void plug::SelectionMask::fill (bool value)
{
    assert (mask && "nullptr filter mask\n");
    for (size_t idx = 0; idx < width_ * height_; ++idx)
    {
        mask[idx] = value;
    }
}


plug::Canvas::Canvas (int width, int height, const plug::Color color, const plug::Vec2d lh_pos, plug::Tool_palette *palette) :
    width_ (width),
    height_ (height),
    color_ (color),
    palette_ (palette),
    selection (width, height)
{
    if (width <= 0 || height <= 0)
    {
        fprintf (stderr, "Error: incorrect size of canvas.\n");
        return;
    }

    layout_ = new plug::Default_layout_box (lh_pos, plug::Vec2d (width, height));
    assert (layout_);

    canvas_texture.create (width, height * 2);
    draw_rect_  = sf::IntRect (0, 0, width, height);

    sf::RectangleShape rect (sf::Vector2f (width, height));
    rect.setFillColor (color);
    
    canvas_texture.draw (rect);
    canvas_texture.display ();
    
    selection.fill (true);
}

plug::Canvas::~Canvas () 
{
    width_  = __INT_MAX__;
    height_ = __INT_MAX__;
    delete layout_;
};

void plug::Canvas::render (sf::RenderTarget &target, plug::TransformStack &transform_stack)
{
    transform_stack.enter (plug::Transform (layout_->get_position ()));
    plug::Vec2d lh_pos = transform_stack.top ().getOffset ();

    sf::Sprite canvas_sprite (canvas_texture.getTexture ());
    canvas_sprite.setPosition (lh_pos);
    canvas_sprite.setTextureRect (draw_rect_);
     
    target.draw (canvas_sprite);
    
    if (palette_ && is_focused)
    {
        plug::Tool *tool = palette_->get_cur_tool ();
        if (tool)
        {
            plug::Widget *widget = tool->get_widget ();
            if (widget)
            {
                widget->render (target, transform_stack);
            }
        }
    }

    transform_stack.leave ();
}

void plug::Canvas::onMousePressed     (const plug::MousePressedEvent &event, plug::EHC &ehc)
{
    plug::Transform tr (layout_->get_position ());
    plug::Transform unite = tr.combine (ehc.stack.top ());
    
    plug::Vec2d pos_ = unite.apply (event.pos);
    if (!contains (pos_.get_x (), pos_.get_y ()))
        return;
    
    if (!palette_)
    {
        return;
    }
    
    plug::Tool *tool = palette_->get_cur_tool ();
    if (tool)
    {
        if (tool->get_widget ())
        {
            // if (tool->get_widget ()->get_layout_box ().get_position ())
            // transform_stack.enter (unite);
            
            tool->get_widget ()->onEvent (event, ehc);
            bool status = ehc.stopped;
            is_focused = status;
            // transform_stack.leave ();
            if (status) 
                return;
        }

        plug::ControlState control_state;
        control_state.state = Pressed;

        tool->setActiveCanvas (*this);
        tool->on_main_button (control_state, pos_);
        is_focused = true;
        ehc.stopped = true;

        return;
    }
}

void plug::Canvas::onMouseReleased    (const plug::MouseReleasedEvent &event, plug::EHC &ehc)
{
    plug::Transform tr (layout_->get_position ());
    plug::Transform unite = tr.combine (ehc.stack.top ()); // stack.enter + stack_top.apply
    plug::Vec2d pos_ = unite.apply (event.pos);

    if (!(palette_ && is_focused))
        return;
    
    plug::Tool *tool = palette_->get_cur_tool ();
    if (tool)
    {
        // ControlState control_state;
        // control_state.state = Released;

        tool->on_confirm ();
        if (!(tool->get_widget ())) is_focused = false;
        ehc.stopped = true;
    }
}

void plug::Canvas::onMouseMove        (const plug::MouseMoveEvent &event, plug::EHC &ehc)
{
    plug::Transform tr (layout_->get_position ());
    plug::Transform unite = tr.combine (ehc.stack.top ());

    plug::Vec2d pos_ = unite.apply (event.pos);

    if (!(palette_ && is_focused))
        return;
    
    plug::Tool *tool = palette_->get_cur_tool ();
    if (tool)
    {
        tool->on_move (pos_);
        // tool->on_modifier_1 (pos_, *this);
        ehc.stopped = true;
    }
}   


void plug::Canvas::onKeyboardPressed  (const plug::KeyboardPressedEvent &event, plug::EHC &ehc)
{
    if (!palette_)
        return;
    
    plug::Tool *tool = palette_->get_cur_tool ();

    if (tool) 
    {
        if (tool->get_widget ())
        {
            tool->get_widget ()->onEvent (event, ehc);
            bool status = ehc.stopped;
            is_focused = status;
            if (status) 
                return;
        }

        switch (event.key_id)
        {
            case Escape:
            {
                tool->on_cancel ();
                ehc.stopped = true;
                return;
            }
            case Enter:
            {
                tool->on_confirm ();
                ehc.stopped = true;
                return;
            }
            case RShift:
            case LShift:
            {
                plug::ControlState state = {Pressed};
                tool->on_modifier_1 (state);
                is_focused = true;
                ehc.stopped = true;
                return;
            }
            default:
            {
                break;
            }
        }
    }
}

void plug::Canvas::onKeyboardReleased (const plug::KeyboardReleasedEvent &event, plug::EHC &ehc)
{
    if (!palette_)
    {
        return;
    }

    plug::Tool *tool = palette_->get_cur_tool ();
    if (!tool)
    {
        return;
    }

    tool->on_released_key ();
    ehc.stopped = true;
}

void plug::Canvas::onTick             (const plug::TickEvent &event, plug::EHC &ehc)
{
    if (!palette_)
    {
        return;
    }

    plug::Tool *tool = palette_->get_cur_tool ();
    if (!(tool && tool->get_widget ()))
    {
        return;
    }

    tool->get_widget ()->onEvent (event, ehc);
}

bool plug::Canvas::contains (int x, int y)
{
    return ((x >= 0 && y >= 0) && 
            (x <= width_ && y <= height_));
}

void plug::Canvas::set_draw_rect_offset (int left, int top)
{
    if (left < 0 || top < 0)
    {
        fprintf (stderr, "Error: %s func has invalid offset parameters.\nHint: left = %d, top = %d.\n", __func__, left, top);
        return;
    }

    draw_rect_.left  = left;
    draw_rect_.top = top;
}

void plug::Canvas::set_draw_rect_size (int width, int height)
{
    if (!(width > 0 && height > 0))
    {
        fprintf (stderr, "Error: %s func has invalid size parameters.\nHint: width = %d, height = %d.\n", __func__, width, height);
        return;
    }

    draw_rect_.width  = width;
    draw_rect_.height = height;
}