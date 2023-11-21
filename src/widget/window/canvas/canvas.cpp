#include "canvas.h"

bool SelectionMask::get_pixel (size_t x, size_t y) const
{
    assert (mask && "nullptr filter mask\n");
    assert ((x < width_) && (y < height_) && "invalid pixel coord\n");

    return mask[x + y * width_];
}  

void SelectionMask::set_pixel (size_t x, size_t y, bool flag)
{
    assert (mask && "nullptr filter mask\n");
    assert ((x < width_) && (y < height_) && "invalid pixel coord\n");

    mask[x + y * width_] = flag;
}  

void SelectionMask::fill (bool value)
{
    assert (mask && "nullptr filter mask\n");
    for (size_t idx = 0; idx < width_ * height_; ++idx)
    {
        mask[idx] = value;
    }
}


Canvas::Canvas (int width, int height, const Color color, const Vec2d lh_pos, Tool_palette *palette) :
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

    layout_ = new Default_layout_box (lh_pos, Vec2d (width, height));
    assert (layout_);

    canvas_texture.create (width, height * 2);
    draw_rect_  = sf::IntRect (0, 0, width, height);

    sf::RectangleShape rect (sf::Vector2f (width, height));
    rect.setFillColor (color);
    
    canvas_texture.draw (rect);
    canvas_texture.display ();
    
    selection.fill (true);
}

Canvas::~Canvas () 
{
    width_  = __INT_MAX__;
    height_ = __INT_MAX__;
    delete layout_;
};

void Canvas::render (sf::RenderTarget &target, TransformStack &transform_stack)
{
    transform_stack.enter (Transform (layout_->get_position ()));
    Vec2d lh_pos = transform_stack.top ().getOffset ();

    sf::Sprite canvas_sprite (canvas_texture.getTexture ());
    canvas_sprite.setPosition (lh_pos);
    canvas_sprite.setTextureRect (draw_rect_);
     
    target.draw (canvas_sprite);
    
    if (palette_ && is_focused)
    {
        Tool *tool = palette_->get_cur_tool ();
        if (tool)
        {
            Widget *widget = tool->get_widget ();
            if (widget)
            {
                widget->render (target, transform_stack);
            }
        }
    }

    transform_stack.leave ();
}

void Canvas::onMousePressed     (const MousePressedEvent &event, EHC &ehc)
{
    Transform tr (layout_->get_position ());
    Transform unite = tr.combine (ehc.stack.top ());
    
    Vec2d pos_ = unite.apply (event.pos);
    if (!contains (pos_.get_x (), pos_.get_y ()))
        return;
    
    if (!palette_)
    {
        return;
    }
    
    Tool *tool = palette_->get_cur_tool ();
    if (tool)
    {
        if (tool->get_widget ())
        {
            // if (tool->get_widget ()->get_layout_box ().get_position ())
            // transform_stack.enter (unite);
            
            tool->get_widget ()->onMousePressed (event, ehc);
            bool status = ehc.stopped;
            is_focused = status;
            // transform_stack.leave ();
            if (status) 
                return;
        }

        ControlState control_state;
        control_state.state = Pressed;

        tool->setActiveCanvas (*this);
        tool->on_main_button (control_state, pos_);
        is_focused = true;
        ehc.stopped = true;

        return;
    }
}

void Canvas::onMouseReleased    (const MouseReleasedEvent &event, EHC &ehc)
{
    Transform tr (layout_->get_position ());
    Transform unite = tr.combine (ehc.stack.top ()); // stack.enter + stack_top.apply
    Vec2d pos_ = unite.apply (event.pos);

    if (!(palette_ && is_focused))
        return;
    
    Tool *tool = palette_->get_cur_tool ();
    if (tool)
    {
        // ControlState control_state;
        // control_state.state = Released;

        tool->on_confirm ();
        if (!(tool->get_widget ())) is_focused = false;
        ehc.stopped = true;
    }
}

void Canvas::onMouseMove        (const MouseMoveEvent &event, EHC &ehc)
{
    Transform tr (layout_->get_position ());
    Transform unite = tr.combine (ehc.stack.top ());

    Vec2d pos_ = unite.apply (event.pos);

    if (!(palette_ && is_focused))
        return;
    
    Tool *tool = palette_->get_cur_tool ();
    if (tool)
    {
        tool->on_move (pos_);
        // tool->on_modifier_1 (pos_, *this);
        ehc.stopped = true;
    }
}   


void Canvas::onKeyboardPressed  (const KeyboardPressedEvent &event, EHC &ehc)
{
    if (!palette_)
        return;
    
    Tool *tool = palette_->get_cur_tool ();

    if (tool) 
    {
        if (tool->get_widget ())
        {
            tool->get_widget ()->onKeyboardPressed (event, ehc);
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
                ControlState state = {Pressed};
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

void Canvas::onKeyboardReleased (const KeyboardReleasedEvent &event, EHC &ehc)
{
    if (!palette_)
    {
        return;
    }

    Tool *tool = palette_->get_cur_tool ();
    if (!tool)
    {
        return;
    }

    tool->on_released_key ();
    ehc.stopped = true;
}

void Canvas::onTick             (const TickEvent &event, EHC &ehc)
{
    if (!palette_)
    {
        return;
    }

    Tool *tool = palette_->get_cur_tool ();
    if (!(tool && tool->get_widget ()))
    {
        return;
    }

    tool->get_widget ()->onTick (event, ehc);
}

bool Canvas::contains (int x, int y)
{
    return ((x >= 0 && y >= 0) && 
            (x <= width_ && y <= height_));
}

void Canvas::set_draw_rect_offset (int left, int top)
{
    if (left < 0 || top < 0)
    {
        fprintf (stderr, "Error: %s func has invalid offset parameters.\nHint: left = %d, top = %d.\n", __func__, left, top);
        return;
    }

    draw_rect_.left  = left;
    draw_rect_.top = top;
}

void Canvas::set_draw_rect_size (int width, int height)
{
    if (!(width > 0 && height > 0))
    {
        fprintf (stderr, "Error: %s func has invalid size parameters.\nHint: width = %d, height = %d.\n", __func__, width, height);
        return;
    }

    draw_rect_.width  = width;
    draw_rect_.height = height;
}