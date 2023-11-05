#include "canvas.h"

Canvas::Canvas (int width, int height, const Color color, const Vector lh_pos, Tool_palette *palette) :
    // transform_ (Transform (lh_pos)),
    width_ (width),
    height_ (height),
    color_ (color),
    palette_ (palette) 
{
    if (width <= 0 || height <= 0)
    {
        fprintf (stderr, "Error: incorrect size of canvas.\n");
        return;
    }

    layout_ = new Default_layout_box (lh_pos, Vector (width, height));
    assert (layout_);

    canvas_texture.create (width, height * 2);
    draw_rect_  = sf::IntRect (0, 0, width, height);

    sf::RectangleShape rect (sf::Vector2f (width, height));
    rect.setFillColor (color);
    
    canvas_texture.draw (rect);
    canvas_texture.display ();
};

Canvas::~Canvas () 
{
    width_  = __INT_MAX__;
    height_ = __INT_MAX__;
    delete layout_;
};

void Canvas::render (sf::RenderTarget &target, Transform_stack &transform_stack)
{
    transform_stack.push (Transform (layout_->get_position ()));
    Vector lh_pos = transform_stack.top ().offset_;

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

    transform_stack.pop ();
}

bool Canvas::on_mouse_pressed  (Mouse_key mouse_key, Vector &pos, Transform_stack &transform_stack)
{
    Transform tr (layout_->get_position ());
    Transform unite = tr.unite (transform_stack.top ());

    Vector pos_ = unite.apply_transform (pos);
    if (!contains (pos_.get_x (), pos_.get_y ()))
        return false;
    
    if (!palette_)
        return false;
    
    Tool *tool = palette_->get_cur_tool ();
    if (tool)
    {
        Button_state state;
        state.pressed = true;
        state.released = false;

        tool->on_main_button (state, pos_, *this);
        is_focused = true;

        return true;
    }

    return false;
}

bool Canvas::on_mouse_released (Mouse_key mouse_key, Vector &pos, Transform_stack &transform_stack)
{
    Transform tr (layout_->get_position ());
    Transform unite = tr.unite (transform_stack.top ());

    Vector pos_ = unite.apply_transform (pos);

    if (!(palette_ && is_focused))
        return false;
    
    Tool *tool = palette_->get_cur_tool ();
    if (tool)
    {
        Button_state state;
        state.pressed = false;
        state.released = true;

        tool->on_confirm (pos_, *this);
        is_focused = false;
        return true;
    }

    return false;
}

bool Canvas::on_mouse_moved    (Vector &new_pos, Transform_stack &transform_stack)
{
    Transform tr (layout_->get_position ());
    Transform unite = tr.unite (transform_stack.top ());

    Vector pos_ = unite.apply_transform (new_pos);

    if (!(palette_ && is_focused))
        return false;
    
    Tool *tool = palette_->get_cur_tool ();
    if (tool)
    {
        tool->on_move (pos_, *this);
        // tool->on_modifier_1 (pos_, *this);
        return true;
    }
    
    return false;
}   

bool Canvas::on_keyboard_pressed  (Keyboard_key key)
{
    if (!palette_)
        return false;
    
    Tool *tool = palette_->get_cur_tool ();
    
    if (tool && key == Escape) 
    {
        switch (key)
        {
            case Escape:
            {
                tool->on_cancel (*this);
                return true;
            }
            case RShift:
            case LShift:
            {
                // tool-> (*this);????
            }
            default:
            {
                break;
            }
        }
    }

    return false;
}

bool Canvas::on_keyboard_released (Keyboard_key key)
{
    return false;
}

bool Canvas::on_tick (float delta_sec)
{
    // TODO
    return false;
}

bool Canvas::contains (int x, int y)
{
    return ((x >= 0 && y >= 0) && 
            (x <= width_ && y <= height_));
}

Color Canvas::get_fg_color ()
{
    if (palette_)
    {
        return palette_->get_fg_color ();
    }
    return Color (0, 0, 0, 0);
}

Color Canvas::get_bg_color ()
{
    if (palette_)
    {
        return palette_->get_bg_color ();
    }
    return Color (0, 0, 0, 0);
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