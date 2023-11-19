#include "tools.h"
#include "../../widget/texture_widget/texture_widget.h"

Brush::Brush () 
{};

Brush::~Brush () 
{};

void Brush::on_main_button         (ControlState &control_state, Vec2d &pos, Canvas &canvas)
{
    //TODO make circle class member (maybe)
    if (control_state.state == Pressed)
    {
        sf::IntRect &rect = canvas.get_draw_rect ();
        pos += Vec2d (rect.left, rect.top);

        sf::CircleShape circle (DEFAULT_BRUSH_THICKNESS);
        circle.setPosition (pos);

        circle.setFillColor (canvas.get_fg_color ());

        prev_pos = pos;

        canvas.canvas_texture.draw (circle);//fg_color
        canvas.canvas_texture.display ();
        is_pressed = true;
    }
}

void Brush::on_secondary_button    (ControlState &control_state, Vec2d &pos, Canvas &canvas)
{
    return;
}

void Brush::on_modifier_1          (Canvas &canvas)
{
    return;
}

void Brush::on_modifier_2          (Canvas &canvas)
{
    return;
}

void Brush::on_modifier_3          (Canvas &canvas)
{
    return;
}

void Brush::on_move                (Vec2d &pos, Canvas &canvas)
{
    if (!is_pressed)
        return;

    sf::IntRect &rect = canvas.get_draw_rect ();
    pos += Vec2d (rect.left, rect.top);

    sf::CircleShape circle (DEFAULT_BRUSH_THICKNESS);
    circle.setPosition (pos);
    circle.setFillColor (canvas.get_fg_color ());   //fg_color
    
    prev_pos = pos;

    canvas.canvas_texture.draw (circle);
    canvas.canvas_texture.display ();
}

void Brush::on_confirm             (Canvas &canvas)
{
    is_pressed = false;
}

void Brush::on_cancel              ()
{

    // sf::CircleShape circle (10);
    // circle.setPosition (prev_pos); 
    // circle.setFillColor (sf::Color::White);   //bg_color
    
    // canvas.canvas_texture.draw (circle);
    // canvas.canvas_texture.display ();

    return;
}  


Line::Line () {};
Line::~Line () {};

void Line::on_main_button         (ControlState &control_state, Vec2d &pos, Canvas &canvas)
{
    if (control_state.state == Pressed)
    {
        Vec2d canvas_size = canvas.get_size ();
        widget_ = new M_render_texture (canvas_size.get_x (), canvas_size.get_y (), Transparent);
        assert (widget_);

        vertex[0] = sf::Vertex (pos, canvas.get_fg_color ());
        M_render_texture *draw_texture = (M_render_texture *)widget_;

        draw_texture->texture_.draw (vertex, 1, sf::Points);
        draw_texture->texture_.display ();
        state_.state = Pressed;
        latest_pos_ = pos;
    }
}

void Line::on_secondary_button    (ControlState &control_state, Vec2d &pos, Canvas &canvas)
{
    return;   
}

void Line::on_modifier_1          (Canvas &canvas)
{
    return;   
}

void Line::on_modifier_2          (Canvas &canvas)
{
    return;   
}

void Line::on_modifier_3          (Canvas &canvas)
{
    return;   
}

void Line::on_move                (Vec2d &pos, Canvas &canvas)
{
    if (state_.state == Released)
        return;

    M_render_texture *draw_texture = (M_render_texture *)widget_;
    assert (draw_texture);
    draw_texture->clear (Transparent);
    
    vertex[1] = sf::Vertex (pos, canvas.get_fg_color ());
    (vertex[0].position == vertex[1].position) ? draw_texture->texture_.draw(&vertex[0], 1, sf::Points) :
                                                 draw_texture->texture_.draw (vertex,    2, sf::Lines);
    draw_texture->texture_.display ();
    
    latest_pos_ = pos;
}

void Line::on_confirm             (Canvas &canvas)
{
    if (state_.state == Released)
        return;

    sf::IntRect &rect = canvas.get_draw_rect ();
    Vec2d draw_rect_offset (rect.left, rect.top);

    vertex[1] = sf::Vertex (latest_pos_ + draw_rect_offset, canvas.get_fg_color ());
    vertex[0].position += sf::Vector2f (draw_rect_offset.get_x (), draw_rect_offset.get_y ());

    (vertex[0].position == vertex[1].position) ? canvas.canvas_texture.draw(&vertex[0], 1, sf::Points) :
                                                 canvas.canvas_texture.draw (vertex,    2, sf::Lines);
    canvas.canvas_texture.display ();

    state_.state = Released;

    delete (M_render_texture *)widget_;
    widget_ = nullptr;

    return;   
}

void Line::on_cancel              ()
{
    if (state_.state == Pressed)
    {
        delete (M_render_texture *)widget_;
        widget_ = nullptr;
        state_.state = Released;
    }
    
    return;   
}


Circle_shape::Circle_shape () {};
Circle_shape::~Circle_shape () {};

void Circle_shape::on_main_button         (ControlState &control_state, Vec2d &pos, Canvas &canvas)
{
    if (control_state.state == Pressed)
    {
        center_ = last_center_ = latest_pos_ = pos;
        
        Vec2d canvas_size = canvas.get_size ();
        widget_ = new M_render_texture (canvas_size.get_x (), canvas_size.get_y (), Transparent);
        assert (widget_);
        
        circle_.setRadius (0);
        circle_.setPosition (center_);
        circle_.setFillColor (Transparent);
        circle_.setOutlineThickness (DEFAULT_CIRCLE_THICKNESS);
        circle_.setOutlineColor (canvas.get_fg_color ());

        state_.state = Pressed;
    }
}

void Circle_shape::on_secondary_button    (ControlState &control_state, Vec2d &pos, Canvas &canvas)
{
    return;
}

void Circle_shape::on_modifier_1          (Canvas &canvas)
{
    if (state_.state == Released)
        return;
    
    is_on_modifier_1_ = true;

    M_render_texture *draw_texture = (M_render_texture *)widget_;
    assert (draw_texture);
    draw_texture->clear (Transparent);

    last_center_ = center_;
    Vec2d pos_offset = latest_pos_ - center_;

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
        last_center_  -= Vec2d (min, 0);
    }
    if (pos_offset.get_y () < 0.0)
    {
        last_center_  -= Vec2d (0, min);
    }

    circle_.setOutlineThickness (DEFAULT_CIRCLE_THICKNESS);

    circle_.setPosition (last_center_);
    circle_.setScale (1, 1);

    draw_texture->texture_.draw (circle_);
    draw_texture->texture_.display ();
}

void Circle_shape::on_modifier_2          (Canvas &canvas)
{
    return;
}

void Circle_shape::on_modifier_3          (Canvas &canvas)
{
    return;
}

void Circle_shape::on_move                (Vec2d &pos, Canvas &canvas)
{
    if (state_.state == Released)
        return;

    latest_pos_ = pos;
    
    if (is_on_modifier_1_)
    {
        on_modifier_1 (canvas);
        return;
    }

    M_render_texture *draw_texture = (M_render_texture *)widget_;
    assert (draw_texture);
    draw_texture->clear (Transparent);

    last_center_ = center_;
    Vec2d pos_offset = pos - center_;

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
        last_center_  += Vec2d (pos_offset.get_x (), 0);
    }
    if (pos_offset.get_y () < 0.0)
    {
        last_center_  += Vec2d (0, pos_offset.get_y ());
    }

    circle_.setOutlineThickness (std::min (DEFAULT_CIRCLE_THICKNESS, MIN_CIRCLE_THICKNESS + min / max));

    circle_.setPosition (last_center_);

    draw_texture->texture_.draw (circle_);
    draw_texture->texture_.display ();
}

void Circle_shape::on_confirm             (Canvas &canvas)
{
    if (state_.state == Released)
        return;

    sf::IntRect &rect = canvas.get_draw_rect ();
    circle_.setPosition (last_center_ + Vec2d (rect.left, rect.top));
    canvas.canvas_texture.draw (circle_);
    canvas.canvas_texture.display ();

    state_.state = Released;

    delete (M_render_texture *)widget_;
    widget_ = nullptr;
}

void Circle_shape::on_cancel              ()
{
    if (state_.state == Pressed)
    {
        delete (M_render_texture *)widget_;
        widget_ = nullptr;
        state_.state = Released;
    }
}

Rect_shape::Rect_shape () {};
Rect_shape::~Rect_shape () {};

void Rect_shape::on_main_button         (ControlState &control_state, Vec2d &pos, Canvas &canvas)
{
    if (control_state.state == Pressed)
    {
        center_ = last_center_ = latest_pos_ = pos;
        
        Vec2d canvas_size = canvas.get_size ();
        widget_ = new M_render_texture (canvas_size.get_x (), canvas_size.get_y (), Transparent);
        assert (widget_);
        
        rect_.setSize (Vec2d (0, 0));
        rect_.setPosition (center_);
        rect_.setFillColor (Transparent);
        rect_.setOutlineThickness (DEFAULT_CIRCLE_THICKNESS);
        rect_.setOutlineColor (canvas.get_fg_color ());

        state_.state = Pressed;
    }
}

void Rect_shape::on_secondary_button    (ControlState &control_state, Vec2d &pos, Canvas &canvas)
{
    return;
}

void Rect_shape::on_modifier_1          (Canvas &canvas)
{
    if (state_.state == Released)
        return;
    
    is_on_modifier_1_ = true;

    M_render_texture *draw_texture = (M_render_texture *)widget_;
    assert (draw_texture);
    draw_texture->clear (Transparent);

    last_center_ = center_;
    Vec2d pos_offset = latest_pos_ - center_;

    double min = 0;
    if (std::abs (pos_offset.get_x ()) < std::abs(pos_offset.get_y ()))
    {
        min = std::abs (pos_offset.get_x ());
    }
    else 
    {
        min = std::abs (pos_offset.get_y ());
    }

    rect_.setSize (Vec2d (min, min));    

    if (pos_offset.get_x () < 0.0)
    {
        last_center_  -= Vec2d (min, 0);
    }
    if (pos_offset.get_y () < 0.0)
    {
        last_center_  -= Vec2d (0, min);
    }

    rect_.setOutlineThickness (DEFAULT_CIRCLE_THICKNESS);

    rect_.setPosition (last_center_);

    draw_texture->texture_.draw (rect_);
    draw_texture->texture_.display ();
}

void Rect_shape::on_modifier_2          (Canvas &canvas)
{
    return;
}

void Rect_shape::on_modifier_3          (Canvas &canvas)
{
    return;
}

void Rect_shape::on_move                (Vec2d &pos, Canvas &canvas)
{
    if (state_.state == Released)
        return;
    
    latest_pos_ = pos;
    
    if (is_on_modifier_1_)
    {
        on_modifier_1 (canvas);
        return;
    }
    
    M_render_texture *draw_texture = (M_render_texture *)widget_;
    assert (draw_texture);
    draw_texture->clear (Transparent);

    last_center_ = center_;
    Vec2d pos_offset = pos - center_;

    if (pos_offset.get_x () < 0.0)
    {
        last_center_  += Vec2d (pos_offset.get_x (), 0);
        pos_offset = Vec2d (std::abs (pos_offset.get_x ()), pos_offset.get_y ());
    }
    if (pos_offset.get_y () < 0.0)
    {
        last_center_  += Vec2d (0, pos_offset.get_y ());
        pos_offset = Vec2d (pos_offset.get_x (), std::abs (pos_offset.get_y ()));
    }
    
    rect_.setSize (pos_offset);    
    rect_.setPosition (last_center_);

    draw_texture->texture_.draw (rect_);
    draw_texture->texture_.display ();
}

void Rect_shape::on_confirm             (Canvas &canvas)
{
    if (state_.state == Released)
        return;

    sf::IntRect &rect = canvas.get_draw_rect ();
    rect_.setPosition (last_center_ + Vec2d (rect.left, rect.top));
    canvas.canvas_texture.draw (rect_);
    canvas.canvas_texture.display ();

    state_.state = Released;
    center_ = last_center_;
    
    delete (M_render_texture *)widget_;
    widget_ = nullptr;
}

void Rect_shape::on_cancel              ()
{
    if (state_.state == Pressed)
    {
        delete (M_render_texture *)widget_;
        widget_ = nullptr;
        state_.state = Released;
    }
}


Fill::Fill () {};
Fill::~Fill () {};

void Fill::on_main_button         (ControlState &control_state, Vec2d &pos, Canvas &canvas)
{
    if (control_state.state == Pressed)
    {
        rect_ = canvas.get_draw_rect ();
        Vec2d real_pos = pos + Vec2d (rect_.left, rect_.top);

        Vec2d canvas_size = canvas.get_size ();
        widget_ = new M_render_texture (canvas_size.get_x (), canvas_size.get_y (), Transparent);
        assert (widget_);

        prev_canvas_img_ = canvas.canvas_texture.getTexture ().copyToImage ();
        size_ = Vec2d (canvas.get_size ().get_x (), canvas.get_size ().get_y ());
        fill_color_ = canvas.get_fg_color ();
        cur_color_  = prev_canvas_img_.getPixel (real_pos.get_x (), real_pos.get_y ());

        pixel_arr_ = (uint8_t *)calloc (4 * size_.get_x () * size_.get_y (), sizeof (uint8_t));
        assert (pixel_arr_);

        fill_pixels (pos, canvas);

        sf::Image new_canvas_img_;
        new_canvas_img_.create (size_.get_x (), size_.get_y (), pixel_arr_);

        draw_texture_.create (size_.get_x (), size_.get_y ());
        draw_texture_.loadFromImage (new_canvas_img_);

        draw_sprite_.setTexture (draw_texture_);
        draw_sprite_.setPosition (0, 0);
        
        ((M_render_texture *)widget_)->texture_.draw (draw_sprite_);
        ((M_render_texture *)widget_)->texture_.display ();

        state_.state = Pressed;
        free (pixel_arr_);
    }

}

void Fill::on_secondary_button    (ControlState &control_state, Vec2d &pos, Canvas &canvas)
{

}

void Fill::on_modifier_1          (Canvas &canvas)
{
    return;
}

void Fill::on_modifier_2          (Canvas &canvas)
{

}

void Fill::on_modifier_3          (Canvas &canvas)
{

}

void Fill::on_move                (Vec2d &pos, Canvas &canvas)
{

}

void Fill::on_confirm             (Canvas &canvas)
{
    if (state_.state == Pressed)
    {
        draw_sprite_.setPosition (rect_.left, rect_.top);
        
        canvas.canvas_texture.draw (draw_sprite_);
        canvas.canvas_texture.display ();

        state_.state = Released;

        delete (M_render_texture *)widget_;
        widget_ = nullptr;
    }
}

void Fill::on_cancel              ()
{
    if (state_.state == Pressed)
    {
        state_.state = Released;
        
        delete (M_render_texture *)widget_;
        widget_ = nullptr;
    }
}

void Fill::fill_pixels (Vec2d &pos, Canvas &canvas)
{
    assert (pixel_arr_);
    M_vector<Vec2d> pixels (Vec2d (0)); // think about init capacity

    Vec2d offset (rect_.left, rect_.top);
    pixels.push (pos);

    while (pixels.get_size ())
    {
        Vec2d cur_pixel = pixels.top ();
        Vec2d real_cur_pixel = cur_pixel + offset;

        pixels.pop ();

        Vec2d left    = (cur_pixel.get_x() - 1 >= 0)                   ? Vec2d (cur_pixel.get_x() - 1, cur_pixel.get_y ()) : Vec2d (-1);
        Vec2d right   = (cur_pixel.get_x() + 1 < (int)size_.get_x ())  ? Vec2d (cur_pixel.get_x() + 1, cur_pixel.get_y ()) : Vec2d (-1);
        Vec2d top     = (cur_pixel.get_y () + 1 < (int)size_.get_y ()) ? Vec2d (cur_pixel.get_x(), cur_pixel.get_y () + 1) : Vec2d (-1);
        Vec2d low     = (cur_pixel.get_y () - 1 >= 0)                  ? Vec2d (cur_pixel.get_x(), cur_pixel.get_y () - 1) : Vec2d (-1);
        
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

        if ((int)left.get_x () >= 0 && (((Color *)pixel_arr_)[(int)left.get_x () + (int)left.get_y () * (int)size_.get_x ()].a == 0) && ((int)cur_pixel.get_x() - 1 >= 0))
        {
            if (cur_color_ == left_color)
            {
                pixels.push (left);
            }
        }
        if ((int)right.get_x () >= 0 && (((Color *)pixel_arr_)[(int)right.get_x () + (int)right.get_y () * (int)size_.get_x ()].a == 0) && ((int)cur_pixel.get_x() + 1 < (int)size_.get_x ()))
        {
            if (cur_color_ == right_color)
            {
                pixels.push (right);
            }
        }
        if ((int)low.get_x () >= 0 && (((Color *)pixel_arr_)[(int)low.get_x () + (int)low.get_y () * (int)size_.get_x ()].a == 0) && ((int)cur_pixel.get_y() - 1 >= 0))
        {
            if (cur_color_ == low_color)
            {
                pixels.push (low);
            }
        }
        if ((int)top.get_x () >= 0 && (((Color *)pixel_arr_)[(int)top.get_x () + (int)top.get_y () * (int)size_.get_x ()].a == 0) && ((int)cur_pixel.get_y() + 1 < (int)size_.get_y ()))
        {
            if (cur_color_ == top_color)
            {
                pixels.push (top);
            }
        }
        
        ((Color *)pixel_arr_)[(int)cur_pixel.get_x () + (int)cur_pixel.get_y () * (int)size_.get_x ()] = fill_color_;
    }
}


void Filter_tool::on_main_button         (ControlState &control_state, Vec2d &pos, Canvas &canvas) 
{
    Filter_mask mask (canvas.canvas_texture.getSize ().x, canvas.canvas_texture.getSize ().y);
    mask.fill (true);
    filter_->apply_filter (canvas, &mask);
}

void Filter_tool::on_secondary_button    (ControlState &control_state, Vec2d &pos, Canvas &canvas) 
{
    return;
}

void Filter_tool::on_modifier_1          (Canvas &canvas) 
{
    return;
}

void Filter_tool::on_modifier_2          (Canvas &canvas) 
{
    return;
}

void Filter_tool::on_modifier_3          (Canvas &canvas) 
{
    return;
}

void Filter_tool::on_move                (Vec2d &pos, Canvas &canvas) 
{
    return;
}

void Filter_tool::on_confirm             (Canvas &canvas) 
{
    return;
}

void Filter_tool::on_cancel              () 
{
    return;
}

Text_tool::Text_tool () {}
Text_tool::~Text_tool () {}

void Text_tool::on_main_button         (ControlState &control_state, Vec2d &pos, Canvas &canvas)
{
    if (state_.state == Released)
    {
        rect_tool.on_main_button (control_state, pos, canvas);
        rect_tool.rect_.setOutlineColor (Black);
        on_rect_ = true;
        state_.state = Pressed;
    }
    // else
    // {
    // }
}

void Text_tool::on_secondary_button    (ControlState &control_state, Vec2d &pos, Canvas &canvas)
{
    return;
}

void Text_tool::on_modifier_1          (Canvas &canvas)
{
    if (on_rect_)
    {
        rect_tool.on_modifier_1 (canvas);
    }
}

void Text_tool::on_modifier_2          (Canvas &canvas)
{
    return;
}

void Text_tool::on_modifier_3          (Canvas &canvas)
{
    return;
}

void Text_tool::on_move                (Vec2d &pos, Canvas &canvas)
{
    if (on_rect_)
    {
        if (!widget_) widget_ = rect_tool.get_widget ();
        rect_tool.on_move (pos, canvas);
        return;
    }
    else
    {
        latest_pos_ = pos;
    }
}

void Text_tool::on_confirm             (Canvas &canvas)
{
    if (on_rect_)
    {
        rect_tool.on_cancel ();
        on_rect_ = false;
        printf ("last_pos = %lf, %lf\n", rect_tool.last_center_.get_x (), rect_tool.last_center_.get_y ());

        widget_ = new M_text (rect_tool.last_center_, rect_tool.rect_.getSize ().x, rect_tool.rect_.getSize ().y, canvas.get_fg_color ());
    }
    else 
    {
        if (!widget_)
        {
            return;
        }
        
        Vec2d size (rect_tool.rect_.getSize ().x, rect_tool.rect_.getSize ().y);
        if ((latest_pos_.get_x () <  rect_tool.center_.get_x ()) || 
            (latest_pos_.get_y () <  rect_tool.center_.get_y ()) || 
            (latest_pos_.get_x () > (rect_tool.center_.get_x () + size.get_x ())) || 
            (latest_pos_.get_y () > (rect_tool.center_.get_y () + size.get_y ())))
        {
            TransformStack stack;

            widget_->render (canvas.canvas_texture, stack);
            canvas.canvas_texture.display ();

            delete (M_text *)widget_;
            widget_ = nullptr;
            
            state_.state = Released;
        } 
    }
}

void Text_tool::on_cancel ()
{
    if (on_rect_)
    {
        on_rect_ = false;
        rect_tool.on_cancel ();
        widget_ = nullptr;
    }
    else 
    {
        rect_tool.on_cancel ();
        if (widget_) delete (M_text *)widget_;
        widget_ = nullptr;
    }
    state_.state = Released;
}