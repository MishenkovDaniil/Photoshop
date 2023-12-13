#include "tools.h"
#include "../../widget/texture_widget/texture_widget.h"
#include "../../catmullRom/catmull.h"


Brush::Brush () {};
Brush::~Brush () {};

void Brush::onMainButton         (const plug::ControlState &control_state, const plug::Vec2d &pos)
{
    assert (active_canvas_);

    if (control_state.state == plug::State::Pressed)
    {
        plug::Vec2d canvas_size = active_canvas_->getSize ();
        widget_ = new M_render_texture (canvas_size.x, canvas_size.y, plug::Transparent);
        assert (widget_);

        CircleShape circle (DEFAULT_BRUSH_THICKNESS);
        circle.setPosition (pos);
        circle.setFillColor (color_palette_->getFGColor ());

        ((M_render_texture *)widget_)->draw (circle); /// fg_color
        
        is_used_ = true;

        key_points_num_ = BrushKeyPointsNum::One;
        last_point[0] = pos;
    }
}

void Brush::onSecondaryButton    (const plug::ControlState &control_state, const plug::Vec2d &pos)
{
    return;
}

void Brush::onModifier1          (const plug::ControlState &control_state)
{
    return;
}

void Brush::onModifier2          (const plug::ControlState &control_state)
{
    return;
}

void Brush::onModifier3          (const plug::ControlState &control_state)
{
    return;
}

void Brush::onMove                (const plug::Vec2d &pos)
{
    if (!is_used_)
        return;
    assert (active_canvas_);

    plug::Color draw_color = color_palette_->getFGColor ();

    switch (key_points_num_)
    {
        case BrushKeyPointsNum::One:
        {
            catmullRomLineDraw (last_point[0], pos, DEFAULT_BRUSH_THICKNESS, 
                                *((M_render_texture *)widget_), draw_color);
            last_point[1] = pos;
            key_points_num_ = BrushKeyPointsNum::Two;
            break;
        }
        case BrushKeyPointsNum::Two:
        {
            catmullRomLeftSplineDraw    (last_point[0], last_point[1], pos, DEFAULT_BRUSH_THICKNESS, 
                                        *((M_render_texture *)widget_), draw_color);
            catmullRomRightSplineDraw   (last_point[0], last_point[1], pos, DEFAULT_BRUSH_THICKNESS, 
                                        *((M_render_texture *)widget_), draw_color);
            last_point[2] = pos;
            key_points_num_ = BrushKeyPointsNum::ThreeOrMore;
            break;
        }
        case BrushKeyPointsNum::ThreeOrMore:
        {
            catmullRomLeftSplineDraw    (last_point[0], last_point[1], pos, DEFAULT_BRUSH_THICKNESS, 
                                        *((M_render_texture *)widget_), draw_color);
            catmullRomRightSplineDraw   (last_point[1], last_point[2], pos, DEFAULT_BRUSH_THICKNESS, 
                                        *((M_render_texture *)widget_), draw_color);
            catmullRomCentralSplineDraw (last_point[0], last_point[1], last_point[2], pos, DEFAULT_BRUSH_THICKNESS, 
                                        *((M_render_texture *)widget_), draw_color);
            last_point[0] = last_point[1];
            last_point[1] = last_point[2];
            last_point[2] = pos;
            break;
        }
        default:
        {
            fprintf (stderr, "Error: bad number of key points in brush tool.\n");
            break;
        }
    }
}

void Brush::onConfirm ()
{
    is_used_ = false;
    
    Sprite &sprite = ((M_render_texture *)widget_)->cur_sprite;
    sprite.setPosition (0, 0);
    ((Canvas *)active_canvas_)->draw (sprite);
    
    delete widget_;
    widget_ = nullptr;

    key_points_num_ = BrushKeyPointsNum::Zero;
}

void Brush::onCancel ()
{
    is_used_ = false;
    delete widget_;
    widget_ = nullptr;
    
    key_points_num_ = BrushKeyPointsNum::Zero;
}  


Line::Line ()  : vertex (plug::Lines, 2) {};
Line::~Line () {};

void Line::onMainButton         (const plug::ControlState &control_state, const plug::Vec2d &pos)
{
    assert (active_canvas_);
    
    if (control_state.state == plug::State::Pressed)
    {
        plug::Vec2d canvas_size = active_canvas_->getSize ();
        widget_ = new M_render_texture (canvas_size.get_x (), canvas_size.get_y (), plug::Transparent);
        assert (widget_);

        vertex[0] = vertex[1] = {pos, plug::Vec2d (), color_palette_->getFGColor ()};
        M_render_texture *draw_texture = (M_render_texture *)widget_;

        draw_texture->draw (vertex);
        latest_pos_ = pos;
    }
}

void Line::onSecondaryButton    (const plug::ControlState &control_state, const plug::Vec2d &pos)
{
    return;   
}

void Line::onModifier1          (const plug::ControlState &control_state)
{
    return;   
}

void Line::onModifier2          (const plug::ControlState &control_state)
{
    return;   
}

void Line::onModifier3          (const plug::ControlState &control_state)
{
    return;   
}

void Line::onMove                (const plug::Vec2d &pos)
{
    if (!widget_)
        return;

    assert (active_canvas_);

    M_render_texture *draw_texture =  (M_render_texture *)widget_;
    assert (draw_texture);
    draw_texture->clear (plug::Transparent);
    
    vertex[1] = {pos, plug::Vec2d (), color_palette_->getFGColor ()};
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

void Line::onConfirm             ()
{
    if (!widget_)
        return;

    assert (active_canvas_);

    vertex[1] = {latest_pos_, plug::Vec2d (), color_palette_->getFGColor ()};
    active_canvas_->draw (vertex);

    delete widget_;
    widget_ = nullptr;
}

void Line::onCancel              ()
{
    if (widget_)
    {
        delete widget_;
        widget_ = nullptr;
    }
}


Circle_shape::Circle_shape () {};
Circle_shape::~Circle_shape () {};

void Circle_shape::onMainButton         (const plug::ControlState &control_state, const plug::Vec2d &pos)
{
    assert (active_canvas_);

    if (control_state.state == plug::State::Pressed)
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
    }
}

void Circle_shape::onSecondaryButton    (const plug::ControlState &control_state, const plug::Vec2d &pos)
{
    return;
}

void Circle_shape::onModifier1          (const plug::ControlState &control_state)
{
    if (!widget_)
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

void Circle_shape::onModifier2          (const plug::ControlState &control_state)
{
    return;
}

void Circle_shape::onModifier3          (const plug::ControlState &control_state)
{
    return;
}

void Circle_shape::onMove                (const plug::Vec2d &pos)
{
    if (!widget_)
        return;

    assert (active_canvas_);

    latest_pos_ = pos;
    
    if (is_on_modifier_1_)
    {
        plug::ControlState state;
        state.state = plug::State::Pressed;
        onModifier1 (state);
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

void Circle_shape::onConfirm             ()
{
    if (!widget_)
        return;
    assert (active_canvas_);

    circle_.setPosition (last_center_ );
    ((Canvas *)active_canvas_)->draw (circle_);

    delete widget_;
    widget_ = nullptr;
}

void Circle_shape::onCancel              ()
{
    if (widget_)
    {
        delete widget_;
        widget_ = nullptr;
    }
}


Rect_shape::Rect_shape () {};
Rect_shape::~Rect_shape () {};

void Rect_shape::onMainButton         (const plug::ControlState &control_state, const plug::Vec2d &pos)
{
    assert (active_canvas_);
    if (control_state.state == plug::State::Pressed)
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
    }
}

void Rect_shape::onSecondaryButton    (const plug::ControlState &control_state, const plug::Vec2d &pos)
{
    return;
}

void Rect_shape::onModifier1          (const plug::ControlState &control_state)
{
    if (!widget_)
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

void Rect_shape::onModifier2          (const plug::ControlState &control_state)
{
    return;
}

void Rect_shape::onModifier3          (const plug::ControlState &control_state)
{
    return;
}

void Rect_shape::onMove                (const plug::Vec2d &pos)
{
    if (!widget_)
        return;

    assert (active_canvas_);

    latest_pos_ = pos;
    
    if (is_on_modifier_1_)
    {
        plug::ControlState state;
        state.state = plug::State::Pressed;
        onModifier1 (state);
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

void Rect_shape::onConfirm             ()
{
    if (!widget_)
        return;

    assert (active_canvas_);

    plug::Texture texture = ((M_render_texture *)widget_)->getTexture ();

    plug::Vec2d canvas_size = active_canvas_->getSize ();
    plug::VertexArray vertices (plug::Quads, 4);
    
    vertices[0].position = plug::Vec2d (0, 0);
    vertices[1].position = plug::Vec2d (0, texture.height);
    vertices[2].position = plug::Vec2d (texture.width, texture.height);
    vertices[3].position = plug::Vec2d (texture.width, 0);

    vertices[0].color = plug::White;
    vertices[1].color = plug::White;
    vertices[2].color = plug::White;
    vertices[3].color = plug::White;
    
    vertices[0].tex_coords = plug::Vec2d (0, 0);
    vertices[1].tex_coords = plug::Vec2d (0, texture.height - 1);
    vertices[2].tex_coords = plug::Vec2d (texture.width - 1, texture.height - 1);
    vertices[3].tex_coords = plug::Vec2d (texture.width - 1, 0);

    active_canvas_->draw (vertices, texture);

    // ((Canvas *)active_canvas_)->draw (rect_);
    center_ = last_center_;
    
    delete widget_;
    widget_ = nullptr;
}

void Rect_shape::onCancel              ()
{
    if (widget_)
    {
        delete (M_render_texture *)widget_;
        widget_ = nullptr;
    }
}


Fill::Fill () {};
Fill::~Fill () {};

void Fill::onMainButton         (const plug::ControlState &control_state, const plug::Vec2d &pos)
{
    assert (active_canvas_);

    if (control_state.state == plug::State::Pressed)
    {
        // plug::Texture texture = active_canvas_->getTexture ();
        plug::Vec2d canvas_size = active_canvas_->getSize ();
        widget_ = new M_render_texture (canvas_size.get_x (), canvas_size.get_y (), plug::Transparent);
        assert (widget_);

        size_ = plug::Vec2d (active_canvas_->getSize ().get_x (), active_canvas_->getSize ().get_y ());
        fill_color_ = color_palette_->getFGColor ();
        cur_color_  = active_canvas_->getPixel (pos.x, pos.y);

        pixel_arr_ = new plug::Color [(int)size_.get_x () * (int)size_.get_y ()];
        assert (pixel_arr_);
        for (int i = 0; i < (int)size_.get_x () * (int)size_.get_y (); ++i)
        {
            pixel_arr_[i] = plug::Transparent;
        }

        fill_pixels (plug::Vec2d (pos.x, pos.y));

        plug::Texture new_canvas_img_ ((size_t)size_.get_x (), (size_t)size_.get_y (), pixel_arr_);

        draw_sprite_.setTexture (new_canvas_img_);
        draw_sprite_.setPosition (0, 0);
        
        ((M_render_texture *)widget_)->draw (draw_sprite_);
    }
}

void Fill::onSecondaryButton    (const plug::ControlState &control_state, const plug::Vec2d &pos)
{
    return;
}

void Fill::onModifier1          (const plug::ControlState &control_state)
{
    return;
}

void Fill::onModifier2          (const plug::ControlState &control_state)
{

}

void Fill::onModifier3          (const plug::ControlState &control_state)
{

}

void Fill::onMove                (const plug::Vec2d &pos)
{
    return;
}

void Fill::onConfirm             ()
{
    assert (active_canvas_);

    if (pixel_arr_)
    {
        ((Canvas *)active_canvas_)->draw (draw_sprite_);

        delete widget_;
        pixel_arr_ = nullptr;
        widget_ = nullptr;
    }
}

void Fill::onCancel              ()
{
    if (pixel_arr_)
    {
        delete[] pixel_arr_;
        pixel_arr_ = nullptr;
    }
    if (widget_)
    {
        delete widget_;
        widget_ = nullptr;
    }
}

void Fill::fill_pixels (plug::Vec2d pos)
{
    assert (active_canvas_);
    assert (pixel_arr_);

    M_vector<plug::Vec2d> pixels (plug::Vec2d (0, 0)); // think about init capacity

    pixels.push (pos);

    while (pixels.get_size ())
    {
        plug::Vec2d cur_pixel = pixels.top ();

        pixels.pop ();

        plug::Vec2d left    = (cur_pixel.get_x() - 1 >= 0)                   ? plug::Vec2d (cur_pixel.get_x() - 1, cur_pixel.get_y ()) : plug::Vec2d (-1);
        plug::Vec2d right   = (cur_pixel.get_x() + 1 < (int)size_.get_x ())  ? plug::Vec2d (cur_pixel.get_x() + 1, cur_pixel.get_y ()) : plug::Vec2d (-1);
        plug::Vec2d top     = (cur_pixel.get_y () + 1 < (int)size_.get_y ()) ? plug::Vec2d (cur_pixel.get_x(), cur_pixel.get_y () + 1) : plug::Vec2d (-1);
        plug::Vec2d low     = (cur_pixel.get_y () - 1 >= 0)                  ? plug::Vec2d (cur_pixel.get_x(), cur_pixel.get_y () - 1) : plug::Vec2d (-1);
        
        plug::Color left_color    = (cur_pixel.get_x() - 1 >= 0) 
                                   ? active_canvas_->getPixel (cur_pixel.get_x() - 1, cur_pixel.get_y ()) 
                                   : plug::Transparent;
        plug::Color right_color   = (cur_pixel.get_x() + 1 < (int)size_.get_x ()) 
                                   ? active_canvas_->getPixel (cur_pixel.get_x() + 1, cur_pixel.get_y ()) 
                                   : plug::Transparent;
        plug::Color top_color     = (cur_pixel.get_y () + 1 < (int)size_.get_y ()) 
                                   ? active_canvas_->getPixel (cur_pixel.get_x(),     cur_pixel.get_y () + 1) 
                                   : plug::Transparent;
        plug::Color low_color     = (cur_pixel.get_y () - 1 >= 0) 
                                   ? active_canvas_->getPixel (cur_pixel.get_x(),     cur_pixel.get_y () - 1) 
                                   : plug::Transparent;

        if ((int)left.get_x () >= 0 && 
            (pixel_arr_[(int)left.get_x () + (int)left.get_y () * (int)size_.get_x ()].a == 0) &&
             ((int)cur_pixel.get_x() - 1 >= 0))
        {
            if (cur_color_.r == left_color.r && 
                cur_color_.g == left_color.g && 
                cur_color_.b == left_color.b && 
                cur_color_.a == left_color.a)
            {
                pixels.push (left);
            }
        }
        if ((int)right.get_x () >= 0 && (pixel_arr_[(int)right.get_x () + (int)right.get_y () * (int)size_.get_x ()].a == 0) && ((int)cur_pixel.get_x() + 1 < (int)size_.get_x ()))
        {
            if (cur_color_.r == right_color.r && 
                cur_color_.g == right_color.g && 
                cur_color_.b == right_color.b && 
                cur_color_.a == right_color.a)
            {
                pixels.push (right);
            }
        }
        if ((int)low.get_x () >= 0 && (pixel_arr_[(int)low.get_x () + (int)low.get_y () * (int)size_.get_x ()].a == 0) && ((int)cur_pixel.get_y() - 1 >= 0))
        {
            if (cur_color_.r == low_color.r && 
                cur_color_.g == low_color.g && 
                cur_color_.b == low_color.b && 
                cur_color_.a == low_color.a)
            {
                pixels.push (low);
            }
        }
        if ((int)top.get_x () >= 0 && (pixel_arr_[(int)top.get_x () + (int)top.get_y () * (int)size_.get_x ()].a == 0) && ((int)cur_pixel.get_y() + 1 < (int)size_.get_y ()))
        {
            if (cur_color_.r == top_color.r && 
                cur_color_.g == top_color.g && 
                cur_color_.b == top_color.b && 
                cur_color_.a == top_color.a)
            {
                pixels.push (top);
            }
        }
        
        pixel_arr_[(int)cur_pixel.get_x () + (int)cur_pixel.get_y () * (int)size_.get_x ()] = fill_color_;
    }
}


void Filter_tool::onMainButton         (const plug::ControlState &control_state, const plug::Vec2d &pos) 
{
    assert (active_canvas_);
    filter_->applyFilter (*active_canvas_);
}

void Filter_tool::onSecondaryButton    (const plug::ControlState &control_state, const plug::Vec2d &pos) 
{
    return;
}

void Filter_tool::onModifier1          (const plug::ControlState &control_state) 
{
    return;
}

void Filter_tool::onModifier2          (const plug::ControlState &control_state) 
{
    return;
}

void Filter_tool::onModifier3          (const plug::ControlState &control_state) 
{
    return;
}

void Filter_tool::onMove                (const plug::Vec2d &pos) 
{
    return;
}

void Filter_tool::onConfirm             () 
{
    return;
}

void Filter_tool::onCancel              () 
{
    return;
}


Text_tool::Text_tool () {}
Text_tool::~Text_tool () {}

void Text_tool::onMainButton         (const plug::ControlState &control_state, const plug::Vec2d &pos)
{
    assert (active_canvas_);
    if (control_state.state == plug::State::Pressed)
    {
        rect_tool.setActiveCanvas (*active_canvas_);
        rect_tool.setColorPalette (*color_palette_);
        rect_tool.onMainButton (control_state, pos);
        rect_tool.rect_.setOutlineColor (plug::Black);
        on_rect_ = true;
    }
}

void Text_tool::onSecondaryButton    (const plug::ControlState &control_state, const plug::Vec2d &pos)
{
    return;
}

void Text_tool::onModifier1          (const plug::ControlState &control_state)
{
    assert (active_canvas_);

    if (on_rect_)
    {
        rect_tool.onModifier1 (control_state);
    }
}

void Text_tool::onModifier2          (const plug::ControlState &control_state)
{
    return;
}

void Text_tool::onModifier3          (const plug::ControlState &control_state)
{
    return;
}

void Text_tool::onMove                (const plug::Vec2d &pos)
{
    assert (active_canvas_);

    if (on_rect_)
    {
        rect_tool.onMove (pos);
        return;
    }
    else
    {
        latest_pos_ = pos;
    }
}

void Text_tool::onConfirm ()
{
    assert (active_canvas_);

    if (on_rect_)
    {
        on_rect_ = false;
        plug::Vec2d rect_size =  rect_tool.rect_.getSize ();
        if (std::abs (rect_size.x) < TEXT_CHARACTER_WIDTH_ || std::abs (rect_size.y) < TEXT_CHARACTER_HEIGHT_)
        {
            rect_tool.onCancel ();
            return;
        }

        rect_tool.onCancel ();

        widget_ = new M_text (rect_tool.last_center_, rect_size.x, rect_size.y, color_palette_->getFGColor ());
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
            plug::Vec2d canvas_size = active_canvas_->getSize ();
            
            TransformStack stack;

            RenderTexture r_texture;
            r_texture.create (canvas_size.x, canvas_size.y);
            r_texture.clear (plug::Transparent);

            widget_->draw (stack, r_texture);
            r_texture.display ();

            plug::Texture texture = r_texture.getTexture ();
            plug::VertexArray vertices (plug::Quads, 4);
            
            vertices[0].position = plug::Vec2d (0, 0);
            vertices[1].position = plug::Vec2d (0, texture.height);
            vertices[2].position = plug::Vec2d (texture.width, texture.height);
            vertices[3].position = plug::Vec2d (texture.width, 0);

            vertices[0].color = plug::White;
            vertices[1].color = plug::White;
            vertices[2].color = plug::White;
            vertices[3].color = plug::White;
            
            vertices[0].tex_coords = plug::Vec2d (0, 0);
            vertices[1].tex_coords = plug::Vec2d (0, texture.height - 1);
            vertices[2].tex_coords = plug::Vec2d (texture.width - 1, texture.height - 1);
            vertices[3].tex_coords = plug::Vec2d (texture.width - 1, 0);

            active_canvas_->draw (vertices, texture);

            delete widget_;
            widget_ = nullptr;
        } 
    }
}

void Text_tool::onCancel ()
{
    assert (active_canvas_);

    if (on_rect_)
    {
        on_rect_ = false;
        rect_tool.onCancel ();
        widget_ = nullptr;
    }
    else 
    {
        rect_tool.onCancel ();
        if (widget_) delete widget_;
        
        widget_ = nullptr;
    }
}


CurveTool::CurveTool (const Curve_filter *filter_, plug::Canvas *filter_canvas) : 
    plot (), 
    filter (filter_),
    filter_canvas_ (filter_canvas)  {}
CurveTool::~CurveTool () 
{
    if (filter)
    {
        delete filter;
        filter = nullptr;
    }
}


void CurveTool::onMainButton         (const plug::ControlState &control_state, const plug::Vec2d &pos)
{
    if (is_called_before == false)
    {
        plug::Vec2d canvas_size = active_canvas_->getSize ();
        plot.create (canvas_size.x, canvas_size.y);
            
        const RenderTexture &r_texture = plot.getRenderTexture ();
        plug::Texture texture = r_texture.getTexture ();

        init_vertices (texture);

        (active_canvas_)->draw (vertices_, texture);
        is_called_before = true;
        return;
    }

    last_point_idx = plot.contains (pos);

    if (last_point_idx == -1)
    {
        plug::Texture texture = plot.getRenderTexture ().getTexture ();
       
        plug::Color color1 = texture.data[(int)(pos.get_x ()) + (int)(pos.get_y ()) * texture.width];
        plug::Color color2 = texture.data[(int)(pos.get_x () + 1) + (int)(pos.get_y ()) * texture.width];
        plug::Color color3 = texture.data[(int)(pos.get_x () + 1) + (int)(pos.get_y () + 1) * texture.width];
       
        if ((color1.r == 255 && color1.g == 255 && color1.b == 255) &&
            (color2.r == 255 && color2.g == 255 && color2.b == 255) &&
            (color3.r == 255 && color3.g == 255 && color3.b == 255))
            return;

        plug::Vertex new_vertex;
        new_vertex.position = pos;
        new_vertex.color = plug::Red;
        last_point_idx = plot.add_key_point (new_vertex);
        // printf ("new_point %d\n", last_point_idx);

        prev_pos_ = pos;
    }
    else 
    {
        prev_pos_ = pos;
        // printf ("contains %d\n", last_point_idx);
    }
}

void CurveTool::onSecondaryButton    (const plug::ControlState &control_state, const plug::Vec2d &pos)
{

}

void CurveTool::onModifier1          (const plug::ControlState &control_state)
{

}

void CurveTool::onModifier2          (const plug::ControlState &control_state)
{

}

void CurveTool::onModifier3          (const plug::ControlState &control_state)
{

}

void CurveTool::onMove                (const plug::Vec2d &pos)
{
    if (last_point_idx < 0)
        return;
    
    // printf ("pos = %lf %lf\n", pos.x, pos.y);

    plug::Vertex new_vertex;
    new_vertex.position = pos;
    new_vertex.color = plug::Red;
    plot.change_key_point (last_point_idx, new_vertex);

    plug::Vec2d canvas_size = active_canvas_->getSize ();
            
    const RenderTexture &r_texture = plot.getRenderTexture ();

    plug::Texture texture = r_texture.getTexture ();

    (active_canvas_)->draw (vertices_, texture);
}

void CurveTool::onConfirm             ()
{
    // printf ("onConfirm\n");
    plug::Vec2d canvas_size = active_canvas_->getSize ();
            
    const RenderTexture &r_texture = plot.getRenderTexture ();

    plug::Texture texture = r_texture.getTexture ();

    (active_canvas_)->draw (vertices_, texture);
    // filter->applyCurveFilter (*filter_canvas_, plot);
}

void CurveTool::onCancel              ()
{
    // printf ("onCancel\n");
}

void CurveTool::init_vertices (plug::Texture &texture)
{
    vertices_[0].position = plug::Vec2d (0, 0);
    vertices_[1].position = plug::Vec2d (0, texture.height);
    vertices_[2].position = plug::Vec2d (texture.width, texture.height);
    vertices_[3].position = plug::Vec2d (texture.width, 0);

    vertices_[0].color = plug::White;
    vertices_[1].color = plug::White;
    vertices_[2].color = plug::White;
    vertices_[3].color = plug::White;
    
    vertices_[0].tex_coords = plug::Vec2d (0, 0);
    vertices_[1].tex_coords = plug::Vec2d (0, texture.height - 1);
    vertices_[2].tex_coords = plug::Vec2d (texture.width - 1, texture.height - 1);
    vertices_[3].tex_coords = plug::Vec2d (texture.width - 1, 0);
}

