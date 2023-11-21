#include "tools.h"
#include "../../widget/texture_widget/texture_widget.h"

plug::Brush::Brush () {};
plug::Brush::~Brush () {};

void plug::Brush::on_main_button         (const plug::ControlState &control_state, plug::Vec2d &pos)
{
    assert (active_canvas_);
    //TODO make circle class member (maybe)
    if (control_state.state == Pressed)
    {
        sf::IntRect &rect = active_canvas_->get_draw_rect ();
        pos += plug::Vec2d (rect.left, rect.top);

        sf::CircleShape circle (DEFAULT_BRUSH_THICKNESS);
        circle.setPosition (pos);

        circle.setFillColor (color_palette_->getFGColor ());

        prev_pos = pos;

        active_canvas_->canvas_texture.draw (circle);//fg_color
        active_canvas_->canvas_texture.display ();
        is_pressed = true;
    }
}

void plug::Brush::on_secondary_button    (const plug::ControlState &control_state, plug::Vec2d &pos)
{
    return;
}

void plug::Brush::on_modifier_1          (const plug::ControlState &control_state)
{
    return;
}

void plug::Brush::on_modifier_2          (const plug::ControlState &control_state)
{
    return;
}

void plug::Brush::on_modifier_3          (const plug::ControlState &control_state)
{
    return;
}

void plug::Brush::on_move                (plug::Vec2d &pos)
{
    if (!is_pressed)
        return;

    assert (active_canvas_);

    sf::IntRect &rect = active_canvas_->get_draw_rect ();
    pos += plug::Vec2d (rect.left, rect.top);

    sf::CircleShape circle (DEFAULT_BRUSH_THICKNESS);
    circle.setPosition (pos);
    circle.setFillColor (color_palette_->getFGColor ());   //fg_color
    
    prev_pos = pos;

    active_canvas_->canvas_texture.draw (circle);
    active_canvas_->canvas_texture.display ();
}

void plug::Brush::on_confirm             ()
{
    is_pressed = false;
}

void plug::Brush::on_cancel              ()
{

    // sf::CircleShape circle (10);
    // circle.setPosition (prev_pos); 
    // circle.setFillColor (sf::Color::White);   //bg_color
    
    // canvas.canvas_texture.draw (circle);
    // canvas.canvas_texture.display ();

    return;
}  


plug::Line::Line () {};
plug::Line::~Line () {};

void plug::Line::on_main_button         (const plug::ControlState &control_state, plug::Vec2d &pos)
{
    assert (active_canvas_);
    
    if (control_state.state == Pressed)
    {
        plug::Vec2d canvas_size = active_canvas_->get_size ();
        widget_ = new plug::M_render_texture (canvas_size.get_x (), canvas_size.get_y (), Transparent);
        assert (widget_);

        vertex[0] = sf::Vertex (pos, color_palette_->getFGColor ());
        plug::M_render_texture *draw_texture = (plug::M_render_texture *)widget_;

        draw_texture->texture_.draw (vertex, 1, sf::Points);
        draw_texture->texture_.display ();
        state_.state = Pressed;
        latest_pos_ = pos;
    }
}

void plug::Line::on_secondary_button    (const plug::ControlState &control_state, plug::Vec2d &pos)
{
    return;   
}

void plug::Line::on_modifier_1          (const plug::ControlState &control_state)
{
    return;   
}

void plug::Line::on_modifier_2          (const plug::ControlState &control_state)
{
    return;   
}

void plug::Line::on_modifier_3          (const plug::ControlState &control_state)
{
    return;   
}

void plug::Line::on_move                (plug::Vec2d &pos)
{
    if (state_.state == Released)
        return;

    assert (active_canvas_);

    plug::M_render_texture *draw_texture = (plug::M_render_texture *)widget_;
    assert (draw_texture);
    draw_texture->clear (Transparent);
    
    vertex[1] = sf::Vertex (pos, color_palette_->getFGColor ());
    (vertex[0].position == vertex[1].position) ? draw_texture->texture_.draw(&vertex[0], 1, sf::Points) :
                                                 draw_texture->texture_.draw (vertex,    2, sf::Lines);
    draw_texture->texture_.display ();
    
    latest_pos_ = pos;
}

void plug::Line::on_confirm             ()
{
    if (state_.state == Released)
        return;

    assert (active_canvas_);

    sf::IntRect &rect = active_canvas_->get_draw_rect ();
    plug::Vec2d draw_rect_offset (rect.left, rect.top);

    vertex[1] = sf::Vertex (latest_pos_ + draw_rect_offset, color_palette_->getFGColor ());
    vertex[0].position += sf::Vector2f (draw_rect_offset.get_x (), draw_rect_offset.get_y ());

    (vertex[0].position == vertex[1].position) ? active_canvas_->canvas_texture.draw(&vertex[0], 1, sf::Points) :
                                                 active_canvas_->canvas_texture.draw (vertex,    2, sf::Lines);
    active_canvas_->canvas_texture.display ();

    state_.state = Released;

    // delete (M_render_texture *)widget_;
    delete widget_;
    widget_ = nullptr;

    return;   
}

void plug::Line::on_cancel              ()
{
    if (state_.state == Pressed)
    {
        delete (M_render_texture *)widget_;
        widget_ = nullptr;
        state_.state = Released;
    }
    
    return;   
}


plug::Circle_shape::Circle_shape () {};
plug::Circle_shape::~Circle_shape () {};

void plug::Circle_shape::on_main_button         (const plug::ControlState &control_state, plug::Vec2d &pos)
{
    assert (active_canvas_);

    if (control_state.state == Pressed)
    {
        center_ = last_center_ = latest_pos_ = pos;
        
        plug::Vec2d canvas_size = active_canvas_->get_size ();
        widget_ = new plug::M_render_texture (canvas_size.get_x (), canvas_size.get_y (), Transparent);
        assert (widget_);
        
        circle_.setRadius (0);
        circle_.setPosition (center_);
        circle_.setFillColor (Transparent);
        circle_.setOutlineThickness (DEFAULT_CIRCLE_THICKNESS);
        circle_.setOutlineColor (color_palette_->getFGColor ());

        state_.state = Pressed;
    }
}

void plug::Circle_shape::on_secondary_button    (const plug::ControlState &control_state, plug::Vec2d &pos)
{
    return;
}

void plug::Circle_shape::on_modifier_1          (const plug::ControlState &control_state)
{
    if (state_.state == Released)
        return;

    assert (active_canvas_);
    
    is_on_modifier_1_ = true;

    plug::M_render_texture *draw_texture = (plug::M_render_texture *)widget_;
    assert (draw_texture);
    draw_texture->clear (Transparent);

    last_center_ = center_;
    plug::Vec2d pos_offset = latest_pos_ - center_;

    double min = 0;
    if (std::abs (pos_offset.get_x ()) < std::abs(pos_offset.get_y ()))
    {
        min = std::abs (pos_offset.get_x ());
    }
    else 
    {
        min = std::abs (pos_offset.get_y ());
    }

    circle_.setRadius (min / 2.0);
        
    if (pos_offset.get_x () < 0.0)
    {
        last_center_  -= plug::Vec2d (min, 0);
    }
    if (pos_offset.get_y () < 0.0)
    {
        last_center_  -= plug::Vec2d (0, min);
    }

    circle_.setOutlineThickness (DEFAULT_CIRCLE_THICKNESS);

    circle_.setPosition (last_center_);
    circle_.setScale (1, 1);

    draw_texture->texture_.draw (circle_);
    draw_texture->texture_.display ();
}

void plug::Circle_shape::on_modifier_2          (const plug::ControlState &control_state)
{
    return;
}

void plug::Circle_shape::on_modifier_3          (const plug::ControlState &control_state)
{
    return;
}

void plug::Circle_shape::on_move                (plug::Vec2d &pos)
{
    if (state_.state == Released)
        return;

    assert (active_canvas_);

    latest_pos_ = pos;
    
    if (is_on_modifier_1_)
    {
        plug::ControlState state;
        state.state = Pressed;
        on_modifier_1 (state);
        return;
    }

    plug::M_render_texture *draw_texture = (plug::M_render_texture *)widget_;
    assert (draw_texture);
    draw_texture->clear (Transparent);

    last_center_ = center_;
    plug::Vec2d pos_offset = pos - center_;

    int min_idx = 0;
    double min = 0;
    double max = 0;
    if (std::abs (pos_offset.get_x ()) < std::abs(pos_offset.get_y ()))
    {
        min = std::abs (pos_offset.get_x ());
        max = std::abs (pos_offset.get_y ());
    }
    else 
    {
        min = std::abs (pos_offset.get_y ());
        max = std::abs (pos_offset.get_x ());
        min_idx = 1;
    }

    circle_.setRadius (min / 2.0);    
    
    if (min)
    {
        if (min_idx == 0)
        {
            circle_.setScale (1, max / min);
        }
        else 
        {
            circle_.setScale (max / min, 1);
        }
    }

    if (pos_offset.get_x () < 0.0)
    {
        last_center_  += plug::Vec2d (pos_offset.get_x (), 0);
    }
    if (pos_offset.get_y () < 0.0)
    {
        last_center_  += plug::Vec2d (0, pos_offset.get_y ());
    }

    circle_.setOutlineThickness (std::min (DEFAULT_CIRCLE_THICKNESS, MIN_CIRCLE_THICKNESS + min / max));

    circle_.setPosition (last_center_);

    draw_texture->texture_.draw (circle_);
    draw_texture->texture_.display ();
}

void plug::Circle_shape::on_confirm             ()
{
    if (state_.state == Released)
        return;

    assert (active_canvas_);

    sf::IntRect &rect = active_canvas_->get_draw_rect ();
    circle_.setPosition (last_center_ + plug::Vec2d (rect.left, rect.top));
    active_canvas_->canvas_texture.draw (circle_);
    active_canvas_->canvas_texture.display ();

    state_.state = Released;

    // delete (M_render_texture *)widget_;
    delete widget_;
    widget_ = nullptr;
}

void plug::Circle_shape::on_cancel              ()
{
    if (state_.state == Pressed)
    {
        // delete (plug::M_render_texture *)widget_;
        delete widget_;
        widget_ = nullptr;
        state_.state = Released;
    }
}


plug::Rect_shape::Rect_shape () {};
plug::Rect_shape::~Rect_shape () {};

void plug::Rect_shape::on_main_button         (const plug::ControlState &control_state, plug::Vec2d &pos)
{
    assert (active_canvas_);

    if (control_state.state == Pressed)
    {
        center_ = last_center_ = latest_pos_ = pos;
        
        plug::Vec2d canvas_size = active_canvas_->get_size ();
        widget_ = new plug::M_render_texture (canvas_size.get_x (), canvas_size.get_y (), Transparent);
        assert (widget_);
        
        rect_.setSize (plug::Vec2d (0, 0));
        rect_.setPosition (center_);
        rect_.setFillColor (Transparent);
        rect_.setOutlineThickness (DEFAULT_CIRCLE_THICKNESS);
        rect_.setOutlineColor (color_palette_->getFGColor ());

        state_.state = Pressed;
    }
}

void plug::Rect_shape::on_secondary_button    (const plug::ControlState &control_state, plug::Vec2d &pos)
{
    return;
}

void plug::Rect_shape::on_modifier_1          (const plug::ControlState &control_state)
{
    if (state_.state == Released)
        return;
    
    assert (active_canvas_);

    is_on_modifier_1_ = true;

    plug::M_render_texture *draw_texture = (plug::M_render_texture *)widget_;
    assert (draw_texture);
    draw_texture->clear (Transparent);

    last_center_ = center_;
    plug::Vec2d pos_offset = latest_pos_ - center_;

    double min = 0;
    if (std::abs (pos_offset.get_x ()) < std::abs(pos_offset.get_y ()))
    {
        min = std::abs (pos_offset.get_x ());
    }
    else 
    {
        min = std::abs (pos_offset.get_y ());
    }

    rect_.setSize (plug::Vec2d (min, min));    

    if (pos_offset.get_x () < 0.0)
    {
        last_center_  -= plug::Vec2d (min, 0);
    }
    if (pos_offset.get_y () < 0.0)
    {
        last_center_  -= plug::Vec2d (0, min);
    }

    rect_.setOutlineThickness (DEFAULT_CIRCLE_THICKNESS);

    rect_.setPosition (last_center_);

    draw_texture->texture_.draw (rect_);
    draw_texture->texture_.display ();
}

void plug::Rect_shape::on_modifier_2          (const plug::ControlState &control_state)
{
    return;
}

void plug::Rect_shape::on_modifier_3          (const plug::ControlState &control_state)
{
    return;
}

void plug::Rect_shape::on_move                (plug::Vec2d &pos)
{
    if (state_.state == Released)
        return;

    assert (active_canvas_);

    latest_pos_ = pos;
    
    if (is_on_modifier_1_)
    {
        plug::ControlState state;
        state.state = Pressed;
        on_modifier_1 (state);
        return;
    }
    
    plug::M_render_texture *draw_texture = (plug::M_render_texture *)widget_;
    assert (draw_texture);
    draw_texture->clear (Transparent);

    last_center_ = center_;
    plug::Vec2d pos_offset = pos - center_;

    if (pos_offset.get_x () < 0.0)
    {
        last_center_  += plug::Vec2d (pos_offset.get_x (), 0);
        pos_offset = plug::Vec2d (std::abs (pos_offset.get_x ()), pos_offset.get_y ());
    }
    if (pos_offset.get_y () < 0.0)
    {
        last_center_  += plug::Vec2d (0, pos_offset.get_y ());
        pos_offset = plug::Vec2d (pos_offset.get_x (), std::abs (pos_offset.get_y ()));
    }
    
    rect_.setSize (pos_offset);    
    rect_.setPosition (last_center_);

    draw_texture->texture_.draw (rect_);
    draw_texture->texture_.display ();
}

void plug::Rect_shape::on_confirm             ()
{
    if (state_.state == Released)
        return;

    assert (active_canvas_);

    sf::IntRect &rect = active_canvas_->get_draw_rect ();
    rect_.setPosition (last_center_ + plug::Vec2d (rect.left, rect.top));
    active_canvas_->canvas_texture.draw (rect_);
    active_canvas_->canvas_texture.display ();

    state_.state = Released;
    center_ = last_center_;
    
    // delete (M_render_texture *)widget_;
    delete widget_;
    widget_ = nullptr;
}

void plug::Rect_shape::on_cancel              ()
{
    if (state_.state == Pressed)
    {
        delete (M_render_texture *)widget_;
        widget_ = nullptr;
        state_.state = Released;
    }
}


plug::Fill::Fill () {};
plug::Fill::~Fill () {};

void plug::Fill::on_main_button         (const plug::ControlState &control_state, plug::Vec2d &pos)
{
    assert (active_canvas_);

    if (control_state.state == Pressed)
    {
        rect_ = active_canvas_->get_draw_rect ();
        plug::Vec2d real_pos = pos + plug::Vec2d (rect_.left, rect_.top);

        plug::Vec2d canvas_size = active_canvas_->get_size ();
        widget_ = new plug::M_render_texture (canvas_size.get_x (), canvas_size.get_y (), Transparent);
        assert (widget_);

        prev_canvas_img_ = active_canvas_->canvas_texture.getTexture ().copyToImage ();
        size_ = plug::Vec2d (active_canvas_->get_size ().get_x (), active_canvas_->get_size ().get_y ());
        fill_color_ = color_palette_->getFGColor ();
        cur_color_  = prev_canvas_img_.getPixel (real_pos.get_x (), real_pos.get_y ());

        pixel_arr_ = (uint8_t *)calloc (4 * size_.get_x () * size_.get_y (), sizeof (uint8_t));
        assert (pixel_arr_);

        fill_pixels (pos);

        sf::Image new_canvas_img_;
        new_canvas_img_.create (size_.get_x (), size_.get_y (), pixel_arr_);

        draw_texture_.create (size_.get_x (), size_.get_y ());
        draw_texture_.loadFromImage (new_canvas_img_);

        draw_sprite_.setTexture (draw_texture_);
        draw_sprite_.setPosition (0, 0);
        
        ((plug::M_render_texture *)widget_)->texture_.draw (draw_sprite_);
        ((plug::M_render_texture *)widget_)->texture_.display ();

        state_.state = Pressed;
        free (pixel_arr_);
    }

}

void plug::Fill::on_secondary_button    (const plug::ControlState &control_state, plug::Vec2d &pos)
{
    return;
}

void plug::Fill::on_modifier_1          (const plug::ControlState &control_state)
{
    return;
}

void plug::Fill::on_modifier_2          (const plug::ControlState &control_state)
{

}

void plug::Fill::on_modifier_3          (const plug::ControlState &control_state)
{

}

void plug::Fill::on_move                (plug::Vec2d &pos)
{
    return;
}

void plug::Fill::on_confirm             ()
{
    assert (active_canvas_);

    if (state_.state == Pressed)
    {
        draw_sprite_.setPosition (rect_.left, rect_.top);
        
        active_canvas_->canvas_texture.draw (draw_sprite_);
        active_canvas_->canvas_texture.display ();

        state_.state = Released;

        // delete (M_render_texture *)widget_;
        delete widget_;
        widget_ = nullptr;
    }
}

void plug::Fill::on_cancel              ()
{
    if (state_.state == Pressed)
    {
        state_.state = Released;
        
        // delete (plug::M_render_texture *)widget_;
        delete widget_;
        widget_ = nullptr;
    }
}

void plug::Fill::fill_pixels (plug::Vec2d &pos)
{
    assert (active_canvas_);
    assert (pixel_arr_);

    M_vector<plug::Vec2d> pixels (plug::Vec2d (0)); // think about init capacity

    plug::Vec2d offset (rect_.left, rect_.top);
    pixels.push (pos);

    while (pixels.get_size ())
    {
        plug::Vec2d cur_pixel = pixels.top ();
        plug::Vec2d real_cur_pixel = cur_pixel + offset;

        pixels.pop ();

        plug::Vec2d left    = (cur_pixel.get_x() - 1 >= 0)                   ? plug::Vec2d (cur_pixel.get_x() - 1, cur_pixel.get_y ()) : plug::Vec2d (-1);
        plug::Vec2d right   = (cur_pixel.get_x() + 1 < (int)size_.get_x ())  ? plug::Vec2d (cur_pixel.get_x() + 1, cur_pixel.get_y ()) : plug::Vec2d (-1);
        plug::Vec2d top     = (cur_pixel.get_y () + 1 < (int)size_.get_y ()) ? plug::Vec2d (cur_pixel.get_x(), cur_pixel.get_y () + 1) : plug::Vec2d (-1);
        plug::Vec2d low     = (cur_pixel.get_y () - 1 >= 0)                  ? plug::Vec2d (cur_pixel.get_x(), cur_pixel.get_y () - 1) : plug::Vec2d (-1);
        
        sf::Color left_color    = (cur_pixel.get_x() - 1 >= 0) 
                                   ? prev_canvas_img_.getPixel (real_cur_pixel.get_x() - 1, real_cur_pixel.get_y ()) 
                                   : Transparent;
        sf::Color right_color   = (cur_pixel.get_x() + 1 < (int)size_.get_x ()) 
                                   ? prev_canvas_img_.getPixel (real_cur_pixel.get_x() + 1, real_cur_pixel.get_y ()) 
                                   : Transparent;
        sf::Color top_color     = (cur_pixel.get_y () + 1 < (int)size_.get_y ()) 
                                   ? prev_canvas_img_.getPixel (real_cur_pixel.get_x(),     real_cur_pixel.get_y () + 1) 
                                   : Transparent;
        sf::Color low_color     = (cur_pixel.get_y () - 1 >= 0) 
                                   ? prev_canvas_img_.getPixel (real_cur_pixel.get_x(),     real_cur_pixel.get_y () - 1) 
                                   : Transparent;

        if ((int)left.get_x () >= 0 && (((plug::Color *)pixel_arr_)[(int)left.get_x () + (int)left.get_y () * (int)size_.get_x ()].a == 0) && ((int)cur_pixel.get_x() - 1 >= 0))
        {
            if (cur_color_ == left_color)
            {
                pixels.push (left);
            }
        }
        if ((int)right.get_x () >= 0 && (((plug::Color *)pixel_arr_)[(int)right.get_x () + (int)right.get_y () * (int)size_.get_x ()].a == 0) && ((int)cur_pixel.get_x() + 1 < (int)size_.get_x ()))
        {
            if (cur_color_ == right_color)
            {
                pixels.push (right);
            }
        }
        if ((int)low.get_x () >= 0 && (((plug::Color *)pixel_arr_)[(int)low.get_x () + (int)low.get_y () * (int)size_.get_x ()].a == 0) && ((int)cur_pixel.get_y() - 1 >= 0))
        {
            if (cur_color_ == low_color)
            {
                pixels.push (low);
            }
        }
        if ((int)top.get_x () >= 0 && (((plug::Color *)pixel_arr_)[(int)top.get_x () + (int)top.get_y () * (int)size_.get_x ()].a == 0) && ((int)cur_pixel.get_y() + 1 < (int)size_.get_y ()))
        {
            if (cur_color_ == top_color)
            {
                pixels.push (top);
            }
        }
        
        ((plug::Color *)pixel_arr_)[(int)cur_pixel.get_x () + (int)cur_pixel.get_y () * (int)size_.get_x ()] = fill_color_;
    }
}


void plug::Filter_tool::on_main_button         (const plug::ControlState &control_state, plug::Vec2d &pos) 
{
    assert (active_canvas_);
    filter_->apply_filter (*active_canvas_);
}

void plug::Filter_tool::on_secondary_button    (const plug::ControlState &control_state, plug::Vec2d &pos) 
{
    return;
}

void plug::Filter_tool::on_modifier_1          (const plug::ControlState &control_state) 
{
    return;
}

void plug::Filter_tool::on_modifier_2          (const plug::ControlState &control_state) 
{
    return;
}

void plug::Filter_tool::on_modifier_3          (const plug::ControlState &control_state) 
{
    return;
}

void plug::Filter_tool::on_move                (plug::Vec2d &pos) 
{
    return;
}

void plug::Filter_tool::on_confirm             () 
{
    return;
}

void plug::Filter_tool::on_cancel              () 
{
    return;
}


plug::Text_tool::Text_tool () {}
plug::Text_tool::~Text_tool () {}

void plug::Text_tool::on_main_button         (const plug::ControlState &control_state, plug::Vec2d &pos)
{
    assert (active_canvas_);
 
    if (state_.state == Released)
    {
        rect_tool.setActiveCanvas (*active_canvas_);
        rect_tool.on_main_button (control_state, pos);
        rect_tool.rect_.setOutlineColor (Black);
        on_rect_ = true;
        state_.state = Pressed;
    }
    // else
    // {
    // }
}

void plug::Text_tool::on_secondary_button    (const plug::ControlState &control_state, plug::Vec2d &pos)
{
    return;
}

void plug::Text_tool::on_modifier_1          (const plug::ControlState &control_state)
{
    assert (active_canvas_);

    if (on_rect_)
    {
        rect_tool.on_modifier_1 (control_state);
    }
}

void plug::Text_tool::on_modifier_2          (const plug::ControlState &control_state)
{
    return;
}

void plug::Text_tool::on_modifier_3          (const plug::ControlState &control_state)
{
    return;
}

void plug::Text_tool::on_move                (plug::Vec2d &pos)
{
    assert (active_canvas_);

    if (on_rect_)
    {
        if (!widget_) widget_ = rect_tool.get_widget ();
        rect_tool.on_move (pos);
        return;
    }
    else
    {
        latest_pos_ = pos;
    }
}

void plug::Text_tool::on_confirm             ()
{
    assert (active_canvas_);

    if (on_rect_)
    {
        rect_tool.on_cancel ();
        on_rect_ = false;
        // printf ("last_pos = %lf, %lf\n", rect_tool.last_center_.get_x (), rect_tool.last_center_.get_y ());

        widget_ = new plug::M_text (rect_tool.last_center_, rect_tool.rect_.getSize ().x, rect_tool.rect_.getSize ().y, color_palette_->getFGColor ());
    }
    else 
    {
        if (!widget_)
        {
            return;
        }
        
        plug::Vec2d size (rect_tool.rect_.getSize ().x, rect_tool.rect_.getSize ().y);
        if ((latest_pos_.get_x () <  rect_tool.center_.get_x ()) || 
            (latest_pos_.get_y () <  rect_tool.center_.get_y ()) || 
            (latest_pos_.get_x () > (rect_tool.center_.get_x () + size.get_x ())) || 
            (latest_pos_.get_y () > (rect_tool.center_.get_y () + size.get_y ())))
        {
            plug::TransformStack stack;

            widget_->render (active_canvas_->canvas_texture, stack);
            active_canvas_->canvas_texture.display ();

            // delete (plug::M_text *)widget_;
            delete widget_;
            widget_ = nullptr;
            
            state_.state = Released;
        } 
    }
}

void plug::Text_tool::on_cancel ()
{
    assert (active_canvas_);

    if (on_rect_)
    {
        on_rect_ = false;
        rect_tool.on_cancel ();
        widget_ = nullptr;
    }
    else 
    {
        rect_tool.on_cancel ();
        if (widget_) delete widget_;
        
        // delete (plug::M_text *)widget_;
        widget_ = nullptr;
    }
    state_.state = Released;
}