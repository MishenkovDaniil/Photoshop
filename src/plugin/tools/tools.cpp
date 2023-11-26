#include "tools.h"
#include "../../widget/texture_widget/texture_widget.h"

Brush::Brush () {};
Brush::~Brush () {};

void Brush::on_main_button         (const plug::ControlState &control_state, plug::Vec2d &pos)
{
    assert (active_canvas_);

    if (control_state.state == plug::Pressed)
    {
        CircleShape circle (DEFAULT_BRUSH_THICKNESS);
        circle.setPosition (pos);

        circle.setFillColor (color_palette_->getFGColor ());

        prev_pos = pos;

        ((Canvas *)active_canvas_)->draw (circle); //fg_color
        is_pressed = true;
    }
}

void Brush::on_secondary_button    (const plug::ControlState &control_state, plug::Vec2d &pos)
{
    return;
}

void Brush::on_modifier_1          (const plug::ControlState &control_state)
{
    return;
}

void Brush::on_modifier_2          (const plug::ControlState &control_state)
{
    return;
}

void Brush::on_modifier_3          (const plug::ControlState &control_state)
{
    return;
}

void Brush::on_move                (plug::Vec2d &pos)
{
    if (!is_pressed)
        return;

    assert (active_canvas_);

    CircleShape circle (DEFAULT_BRUSH_THICKNESS);
    circle.setPosition (pos);
    circle.setFillColor (color_palette_->getFGColor ());   //fg_color
    prev_pos = pos;

    ((Canvas *)active_canvas_)->draw (circle); //fg_color
}

void Brush::on_confirm             ()
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

void Line::on_main_button         (const plug::ControlState &control_state, plug::Vec2d &pos)
{
    assert (active_canvas_);
    
    if (control_state.state == plug::Pressed)
    {
        plug::Vec2d canvas_size = active_canvas_->getSize ();
        widget_ = new M_render_texture (canvas_size.get_x (), canvas_size.get_y (), plug::Transparent);
        assert (widget_);

        vertex[0] = vertex[1] = {pos, color_palette_->getFGColor (), plug::Vec2d ()};
        M_render_texture *draw_texture = (M_render_texture *)widget_;

        draw_texture->draw (vertex);
        state_.state = plug::Pressed;
        latest_pos_ = pos;
    }
}

void Line::on_secondary_button    (const plug::ControlState &control_state, plug::Vec2d &pos)
{
    return;   
}

void Line::on_modifier_1          (const plug::ControlState &control_state)
{
    return;   
}

void Line::on_modifier_2          (const plug::ControlState &control_state)
{
    return;   
}

void Line::on_modifier_3          (const plug::ControlState &control_state)
{
    return;   
}

void Line::on_move                (plug::Vec2d &pos)
{
    if (state_.state == plug::Released)
        return;

    assert (active_canvas_);

    M_render_texture *draw_texture =  (M_render_texture *)widget_;
    assert (draw_texture);
    draw_texture->clear (plug::Transparent);
    
    vertex[1] = {pos, color_palette_->getFGColor (), plug::Vec2d ()};
    if ((vertex[0].position.x == vertex[1].position.x) && 
        (vertex[0].position.y == vertex[1].position.y)) 
    {
        vertex.setPrimitive (plug::Points);
    }
    else
    {
        vertex.setPrimitive (plug::Lines);        
    }

    draw_texture->draw(vertex);
    
    latest_pos_ = pos;
}

void Line::on_confirm             ()
{
    if (state_.state == plug::Released)
        return;

    assert (active_canvas_);

    vertex[1] = {latest_pos_, color_palette_->getFGColor (), plug::Vec2d ()};
    active_canvas_->draw (vertex);

    state_.state = plug::Released;

    delete widget_;
    widget_ = nullptr;

    return;   
}

void Line::on_cancel              ()
{
    if (state_.state == plug::Pressed)
    {
        delete (M_render_texture *)widget_;
        widget_ = nullptr;
        state_.state = plug::Released;
    }
    
    return;   
}


Circle_shape::Circle_shape () {};
Circle_shape::~Circle_shape () {};

void Circle_shape::on_main_button         (const plug::ControlState &control_state, plug::Vec2d &pos)
{
    assert (active_canvas_);

    if (control_state.state == plug::Pressed)
    {
        center_ = last_center_ = latest_pos_ = pos;
        
        plug::Vec2d canvas_size = active_canvas_->getSize ();
        widget_ = new M_render_texture (canvas_size.get_x (), canvas_size.get_y (), plug::Transparent);
        assert (widget_);
        
        circle_.setRadius (0);
        circle_.setPosition (center_);
        circle_.setFillColor (plug::Transparent);
        circle_.setOutlineThickness (DEFAULT_CIRCLE_THICKNESS);
        circle_.setOutlineColor (color_palette_->getFGColor ());

        state_.state = plug::Pressed;
    }
}

void Circle_shape::on_secondary_button    (const plug::ControlState &control_state, plug::Vec2d &pos)
{
    return;
}

void Circle_shape::on_modifier_1          (const plug::ControlState &control_state)
{
    if (state_.state == plug::Released)
        return;

    assert (active_canvas_);
    
    is_on_modifier_1_ = true;

    M_render_texture *draw_texture =  (M_render_texture *)widget_;
    assert (draw_texture);
    draw_texture->clear (plug::Transparent);

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
    circle_.setScale (plug::Vec2d (1, 1));

    draw_texture->draw (circle_);
}

void Circle_shape::on_modifier_2          (const plug::ControlState &control_state)
{
    return;
}

void Circle_shape::on_modifier_3          (const plug::ControlState &control_state)
{
    return;
}

void Circle_shape::on_move                (plug::Vec2d &pos)
{
    if (state_.state == plug::Released)
        return;

    assert (active_canvas_);

    latest_pos_ = pos;
    
    if (is_on_modifier_1_)
    {
        plug::ControlState state;
        state.state = plug::Pressed;
        on_modifier_1 (state);
        return;
    }

    M_render_texture *draw_texture =  (M_render_texture *)widget_;
    assert (draw_texture);
    draw_texture->clear (plug::Transparent);

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
            circle_.setScale (plug::Vec2d (1, max / min));
        }
        else 
        {
            circle_.setScale (plug::Vec2d (max / min, 1));
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

    draw_texture->draw (circle_);
}

void Circle_shape::on_confirm             ()
{
    if (state_.state == plug::Released)
        return;

    assert (active_canvas_);

    circle_.setPosition (last_center_ );
    ((Canvas *)active_canvas_)->draw (circle_);

    state_.state = plug::Released;

    delete widget_;
    widget_ = nullptr;
}

void Circle_shape::on_cancel              ()
{
    if (state_.state == plug::Pressed)
    {
        delete widget_;
        widget_ = nullptr;
        state_.state = plug::Released;
    }
}


Rect_shape::Rect_shape () {};
Rect_shape::~Rect_shape () {};

void Rect_shape::on_main_button         (const plug::ControlState &control_state, plug::Vec2d &pos)
{
    assert (active_canvas_);

    if (control_state.state == plug::Pressed)
    {
        center_ = last_center_ = latest_pos_ = pos;
        
        plug::Vec2d canvas_size = active_canvas_->getSize ();
        widget_ = new M_render_texture (canvas_size.get_x (), canvas_size.get_y (), plug::Transparent);
        assert (widget_);
        
        rect_.setSize (plug::Vec2d (0, 0));
        rect_.setPosition (center_);
        rect_.setFillColor (plug::Transparent);
        rect_.setOutlineThickness (DEFAULT_CIRCLE_THICKNESS);
        rect_.setOutlineColor (color_palette_->getFGColor ());

        state_.state = plug::Pressed;
    }
}

void Rect_shape::on_secondary_button    (const plug::ControlState &control_state, plug::Vec2d &pos)
{
    return;
}

void Rect_shape::on_modifier_1          (const plug::ControlState &control_state)
{
    if (state_.state == plug::Released)
        return;
    
    assert (active_canvas_);

    is_on_modifier_1_ = true;

    M_render_texture *draw_texture =  (M_render_texture *)widget_;
    assert (draw_texture);
    draw_texture->clear (plug::Transparent);

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

    draw_texture->draw (rect_);
}

void Rect_shape::on_modifier_2          (const plug::ControlState &control_state)
{
    return;
}

void Rect_shape::on_modifier_3          (const plug::ControlState &control_state)
{
    return;
}

void Rect_shape::on_move                (plug::Vec2d &pos)
{
    if (state_.state == plug::Released)
        return;

    assert (active_canvas_);

    latest_pos_ = pos;
    
    if (is_on_modifier_1_)
    {
        plug::ControlState state;
        state.state = plug::Pressed;
        on_modifier_1 (state);
        return;
    }
    
    M_render_texture *draw_texture =  (M_render_texture *)widget_;
    assert (draw_texture);
    draw_texture->clear (plug::Transparent);

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

    draw_texture->draw (rect_);
}

void Rect_shape::on_confirm             ()
{
    if (state_.state == plug::Released)
        return;

    assert (active_canvas_);

    ((Canvas *)active_canvas_)->draw (rect_);

    state_.state = plug::Released;
    center_ = last_center_;
    
    delete widget_;
    widget_ = nullptr;
}

void Rect_shape::on_cancel              ()
{
    if (state_.state == plug::Pressed)
    {
        delete (M_render_texture *)widget_;
        widget_ = nullptr;
        state_.state = plug::Released;
    }
}


Fill::Fill () {};
Fill::~Fill () {};

void Fill::on_main_button         (const plug::ControlState &control_state, plug::Vec2d &pos)
{
    assert (active_canvas_);

    // if (control_state.state == plug::Pressed)
    // {
    //     rect_ = active_canvas_->getDrawRect ();
    //     plug::Vec2d real_pos = pos + plug::Vec2d (rect_.left, rect_.top);

    //     plug::Vec2d canvas_size = active_canvas_->getSize ();
    //     widget_ = new M_render_texture (canvas_size.get_x (), canvas_size.get_y (), plug::Transparent);
    //     assert (widget_);

    //     prev_canvas_img_ = active_canvas_->getRenderTexture ().getTexture ().copyToImage ();
    //     size_ = plug::Vec2d (active_canvas_->getSize ().get_x (), active_canvas_->getSize ().get_y ());
    //     fill_color_ = color_palette_->getFGColor ();
    //     cur_color_  = prev_canvas_img_.getPixel (real_pos.get_x (), real_pos.get_y ());

    //     pixel_arr_ = (uint8_t *)calloc (4 * size_.get_x () * size_.get_y (), sizeof (uint8_t));
    //     assert (pixel_arr_);

    //     fill_pixels (pos);

    //     sf::Image new_canvas_img_;
    //     new_canvas_img_.create (size_.get_x (), size_.get_y (), pixel_arr_);

    //     draw_texture_.create (size_.get_x (), size_.get_y ());
    //     draw_texture_.loadFromImage (new_canvas_img_);

    //     draw_sprite_.setTexture (draw_texture_);
    //     draw_sprite_.setPosition (0, 0);
        
    //     ((M_render_texture *)widget_)->texture_.draw (draw_sprite_);
    //     ((M_render_texture *)widget_)->texture_.display ();

    //     state_.state = plug::Pressed;
    //     free (pixel_arr_);
    // }

}

void Fill::on_secondary_button    (const plug::ControlState &control_state, plug::Vec2d &pos)
{
    return;
}

void Fill::on_modifier_1          (const plug::ControlState &control_state)
{
    return;
}

void Fill::on_modifier_2          (const plug::ControlState &control_state)
{

}

void Fill::on_modifier_3          (const plug::ControlState &control_state)
{

}

void Fill::on_move                (plug::Vec2d &pos)
{
    return;
}

void Fill::on_confirm             ()
{
    assert (active_canvas_);

    // if (state_.state == plug::Pressed)
    // {
    //     draw_sprite_.setPosition (rect_.left, rect_.top);
        
    //     active_canvas_->getRenderTexture ().draw (draw_sprite_);
    //     active_canvas_->getRenderTexture ().display ();

    //     state_.state = plug::Released;

    //     // delete (M_render_texture *)widget_;
    //     delete widget_;
    //     widget_ = nullptr;
    // }
}

void Fill::on_cancel              ()
{
    if (state_.state == plug::Pressed)
    {
        state_.state = plug::Released;
        
        // delete  M_render_texture *)widget_;
        delete widget_;
        widget_ = nullptr;
    }
}

void Fill::fill_pixels (plug::Vec2d &pos)
{
    // assert (active_canvas_);
    // assert (pixel_arr_);

    // M_vector<plug::Vec2d> pixels (plug::Vec2d (0)); // think about init capacity

    // plug::Vec2d offset (rect_.left, rect_.top);
    // pixels.push (pos);

    // while (pixels.get_size ())
    // {
    //     plug::Vec2d cur_pixel = pixels.top ();
    //     plug::Vec2d real_cur_pixel = cur_pixel + offset;

    //     pixels.pop ();

    //     plug::Vec2d left    = (cur_pixel.get_x() - 1 >= 0)                   ? plug::Vec2d (cur_pixel.get_x() - 1, cur_pixel.get_y ()) : plug::Vec2d (-1);
    //     plug::Vec2d right   = (cur_pixel.get_x() + 1 < (int)size_.get_x ())  ? plug::Vec2d (cur_pixel.get_x() + 1, cur_pixel.get_y ()) : plug::Vec2d (-1);
    //     plug::Vec2d top     = (cur_pixel.get_y () + 1 < (int)size_.get_y ()) ? plug::Vec2d (cur_pixel.get_x(), cur_pixel.get_y () + 1) : plug::Vec2d (-1);
    //     plug::Vec2d low     = (cur_pixel.get_y () - 1 >= 0)                  ? plug::Vec2d (cur_pixel.get_x(), cur_pixel.get_y () - 1) : plug::Vec2d (-1);
        
    //     sf::Color left_color    = (cur_pixel.get_x() - 1 >= 0) 
    //                                ? prev_canvas_img_.getPixel (real_cur_pixel.get_x() - 1, real_cur_pixel.get_y ()) 
    //                                : plug::Transparent;
    //     sf::Color right_color   = (cur_pixel.get_x() + 1 < (int)size_.get_x ()) 
    //                                ? prev_canvas_img_.getPixel (real_cur_pixel.get_x() + 1, real_cur_pixel.get_y ()) 
    //                                : plug::Transparent;
    //     sf::Color top_color     = (cur_pixel.get_y () + 1 < (int)size_.get_y ()) 
    //                                ? prev_canvas_img_.getPixel (real_cur_pixel.get_x(),     real_cur_pixel.get_y () + 1) 
    //                                : plug::Transparent;
    //     sf::Color low_color     = (cur_pixel.get_y () - 1 >= 0) 
    //                                ? prev_canvas_img_.getPixel (real_cur_pixel.get_x(),     real_cur_pixel.get_y () - 1) 
    //                                : plug::Transparent;

    //     if ((int)left.get_x () >= 0 && (((plug::Color *)pixel_arr_)[(int)left.get_x () + (int)left.get_y () * (int)size_.get_x ()].a == 0) && ((int)cur_pixel.get_x() - 1 >= 0))
    //     {
    //         if (cur_color_ == left_color)
    //         {
    //             pixels.push (left);
    //         }
    //     }
    //     if ((int)right.get_x () >= 0 && (((plug::Color *)pixel_arr_)[(int)right.get_x () + (int)right.get_y () * (int)size_.get_x ()].a == 0) && ((int)cur_pixel.get_x() + 1 < (int)size_.get_x ()))
    //     {
    //         if (cur_color_ == right_color)
    //         {
    //             pixels.push (right);
    //         }
    //     }
    //     if ((int)low.get_x () >= 0 && (((plug::Color *)pixel_arr_)[(int)low.get_x () + (int)low.get_y () * (int)size_.get_x ()].a == 0) && ((int)cur_pixel.get_y() - 1 >= 0))
    //     {
    //         if (cur_color_ == low_color)
    //         {
    //             pixels.push (low);
    //         }
    //     }
    //     if ((int)top.get_x () >= 0 && (((plug::Color *)pixel_arr_)[(int)top.get_x () + (int)top.get_y () * (int)size_.get_x ()].a == 0) && ((int)cur_pixel.get_y() + 1 < (int)size_.get_y ()))
    //     {
    //         if (cur_color_ == top_color)
    //         {
    //             pixels.push (top);
    //         }
    //     }
        
    //     ((plug::Color *)pixel_arr_)[(int)cur_pixel.get_x () + (int)cur_pixel.get_y () * (int)size_.get_x ()] = fill_color_;
    // }
}


void Filter_tool::on_main_button         (const plug::ControlState &control_state, plug::Vec2d &pos) 
{
    assert (active_canvas_);
    filter_->apply_filter (*active_canvas_);
}

void Filter_tool::on_secondary_button    (const plug::ControlState &control_state, plug::Vec2d &pos) 
{
    return;
}

void Filter_tool::on_modifier_1          (const plug::ControlState &control_state) 
{
    return;
}

void Filter_tool::on_modifier_2          (const plug::ControlState &control_state) 
{
    return;
}

void Filter_tool::on_modifier_3          (const plug::ControlState &control_state) 
{
    return;
}

void Filter_tool::on_move                (plug::Vec2d &pos) 
{
    return;
}

void Filter_tool::on_confirm             () 
{
    return;
}

void Filter_tool::on_cancel              () 
{
    return;
}


Text_tool::Text_tool () {}
Text_tool::~Text_tool () {}

void Text_tool::on_main_button         (const plug::ControlState &control_state, plug::Vec2d &pos)
{
    assert (active_canvas_);
 
    if (state_.state == plug::Released)
    {
        rect_tool.setActiveCanvas (*active_canvas_);
        rect_tool.on_main_button (control_state, pos);
        rect_tool.rect_.setOutlineColor (plug::Black);
        on_rect_ = true;
        state_.state = plug::Pressed;
    }
    // else
    // {
    // }
}

void Text_tool::on_secondary_button    (const plug::ControlState &control_state, plug::Vec2d &pos)
{
    return;
}

void Text_tool::on_modifier_1          (const plug::ControlState &control_state)
{
    assert (active_canvas_);

    if (on_rect_)
    {
        rect_tool.on_modifier_1 (control_state);
    }
}

void Text_tool::on_modifier_2          (const plug::ControlState &control_state)
{
    return;
}

void Text_tool::on_modifier_3          (const plug::ControlState &control_state)
{
    return;
}

void Text_tool::on_move                (plug::Vec2d &pos)
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

void Text_tool::on_confirm             ()
{
    assert (active_canvas_);

    // if (on_rect_)
    // {
    //     rect_tool.on_cancel ();
    //     on_rect_ = false;
    //     // printf ("last_pos = %lf, %lf\n", rect_tool.last_center_.get_x (), rect_tool.last_center_.get_y ());

    //     widget_ = new M_text (rect_tool.last_center_, rect_tool.rect_.getSize ().x, rect_tool.rect_.getSize ().y, color_palette_->getFGColor ());
    // }
    // else 
    // {
    //     if (!widget_)
    //     {
    //         return;
    //     }
        
    //     plug::Vec2d size (rect_tool.rect_.getSize ().x, rect_tool.rect_.getSize ().y);
    //     if ((latest_pos_.get_x () <  rect_tool.center_.get_x ()) || 
    //         (latest_pos_.get_y () <  rect_tool.center_.get_y ()) || 
    //         (latest_pos_.get_x () > (rect_tool.center_.get_x () + size.get_x ())) || 
    //         (latest_pos_.get_y () > (rect_tool.center_.get_y () + size.get_y ())))
    //     {
    //         TransformStack stack;

    //         widget_->render (active_canvas_->getRenderTexture (), stack);
    //         active_canvas_->getRenderTexture ().display ();

    //         // delete (M_text *)widget_;
    //         delete widget_;
    //         widget_ = nullptr;
            
    //         state_.state = plug::Released;
    //     } 
    // }
}

void Text_tool::on_cancel ()
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
        
        // delete (M_text *)widget_;
        widget_ = nullptr;
    }
    state_.state = plug::Released;
}