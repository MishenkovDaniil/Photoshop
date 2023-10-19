#include "tools.h"
#include "../../texture_widget/texture_widget.h"

Brush::Brush () 
{};

Brush::~Brush () 
{};

void Brush::on_main_button         (Button_state &state, Vector &pos, Canvas &canvas)
{
    //TODO make circle class member (maybe)
    if (state.pressed)
    {
        pos += Vector (canvas.draw_rect_.left, canvas.draw_rect_.top);
        sf::CircleShape circle (DEFAULT_BRUSH_THICKNESS);
        circle.setPosition (pos);

        circle.setFillColor (canvas.get_fg_color ());

        prev_pos = pos;

        canvas.canvas_texture.draw (circle);//fg_color
        canvas.canvas_texture.display ();
        is_pressed = true;
    }
}

void Brush::on_secondary_button    (Button_state &state, Vector &pos, Canvas &canvas)
{
    return;
}

void Brush::on_modifier_1          (Vector &pos, Canvas &canvas)
{
    return;
}

void Brush::on_modifier_2          (Vector &pos, Canvas &canvas)
{
    return;
}

void Brush::on_modifier_3          (Vector &pos, Canvas &canvas)
{
    return;
}

void Brush::on_move                (Vector &pos, Canvas &canvas)
{
    if (!is_pressed)
        return;

    pos += Vector (canvas.draw_rect_.left, canvas.draw_rect_.top);

    sf::CircleShape circle (DEFAULT_BRUSH_THICKNESS);
    circle.setPosition (pos);
    circle.setFillColor (canvas.get_fg_color ());   //fg_color
    
    prev_pos = pos;

    canvas.canvas_texture.draw (circle);
    canvas.canvas_texture.display ();
}

void Brush::on_confirm             (Vector &pos, Canvas &canvas)
{
    is_pressed = false;
}

void Brush::on_cancel              (Canvas &canvas)
{

    sf::CircleShape circle (10);
    circle.setPosition (prev_pos); 
    circle.setFillColor (sf::Color::White);   //bg_color
    
    canvas.canvas_texture.draw (circle);
    canvas.canvas_texture.display ();

    return;
}  


Line::Line () {};
Line::~Line () {};

void Line::on_main_button         (Button_state &state, Vector &pos, Canvas &canvas)
{
    if (state.pressed)
    {
        Vector canvas_size = canvas.get_size ();
        widget_ = new M_render_texture (canvas_size.get_x (), canvas_size.get_y (), Transparent);
        assert (widget_);

        vertex[0] = sf::Vertex (pos, canvas.get_fg_color ());
        M_render_texture *draw_texture = (M_render_texture *)widget_;

        draw_texture->texture_.draw (vertex, 1, sf::Points);
        draw_texture->texture_.display ();
        state_.pressed = true;
    }
}

void Line::on_secondary_button    (Button_state &state, Vector &pos, Canvas &canvas)
{
    return;   
}

void Line::on_modifier_1          (Vector &pos, Canvas &canvas)
{
    return;   
}

void Line::on_modifier_2          (Vector &pos, Canvas &canvas)
{
    return;   
}

void Line::on_modifier_3          (Vector &pos, Canvas &canvas)
{
    return;   
}

void Line::on_move                (Vector &pos, Canvas &canvas)
{
    if (!state_.pressed)
        return;
    M_render_texture *draw_texture = (M_render_texture *)widget_;
    assert (draw_texture);
    draw_texture->clear (Transparent);
    
    vertex[1] = sf::Vertex (pos, canvas.get_fg_color ());
    (vertex[0].position == vertex[1].position) ? draw_texture->texture_.draw(&vertex[0], 1, sf::Points) :
                                                 draw_texture->texture_.draw (vertex,    2, sf::Lines);
    draw_texture->texture_.display ();

    return;   
}

void Line::on_confirm             (Vector &pos, Canvas &canvas)
{
    if (!state_.pressed)
        return;

    Vector draw_rect_offset (canvas.draw_rect_.left, canvas.draw_rect_.top);

    vertex[1] = sf::Vertex (pos + draw_rect_offset, canvas.get_fg_color ());
    vertex[0].position += sf::Vector2f (draw_rect_offset.get_x (), draw_rect_offset.get_y ());

    (vertex[0].position == vertex[1].position) ? canvas.canvas_texture.draw(&vertex[0], 1, sf::Points) :
                                                 canvas.canvas_texture.draw (vertex,    2, sf::Lines);
    canvas.canvas_texture.display ();

    state_.pressed = false;

    delete (M_render_texture *)widget_;
    widget_ = nullptr;

    return;   
}

void Line::on_cancel              (Canvas &canvas)
{
    if (state_.pressed)
    {
        delete (M_render_texture *)widget_;
        widget_ = nullptr;
        state_.pressed = false;
    }
    
    return;   
}


Circle_shape::Circle_shape () {};
Circle_shape::~Circle_shape () {};

void Circle_shape::on_main_button         (Button_state &state, Vector &pos, Canvas &canvas)
{
    if (state.pressed)
    {
        center_ = last_center_ = pos;
        
        Vector canvas_size = canvas.get_size ();
        widget_ = new M_render_texture (canvas_size.get_x (), canvas_size.get_y (), Transparent);
        assert (widget_);
        
        circle_.setPosition (center_);
        circle_.setFillColor (Transparent);
        circle_.setOutlineThickness (DEFAULT_CIRCLE_THICKNESS);
        circle_.setOutlineColor (canvas.get_fg_color ());

        state_.pressed = true;
    }
}

void Circle_shape::on_secondary_button    (Button_state &state, Vector &pos, Canvas &canvas)
{
    return;
}

void Circle_shape::on_modifier_1          (Vector &pos, Canvas &canvas)
{
    if (!state_.pressed)
        return;
    
    M_render_texture *draw_texture = (M_render_texture *)widget_;
    assert (draw_texture);
    draw_texture->clear (Transparent);

    last_center_ = center_;
    Vector pos_offset = pos - center_;

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
        last_center_  -= Vector (min, 0);
    }
    if (pos_offset.get_y () < 0.0)
    {
        last_center_  -= Vector (0, min);
    }

    circle_.setOutlineThickness (DEFAULT_CIRCLE_THICKNESS);

    circle_.setPosition (last_center_);

    draw_texture->texture_.draw (circle_);
    draw_texture->texture_.display ();
}

void Circle_shape::on_modifier_2          (Vector &pos, Canvas &canvas)
{
    return;
}

void Circle_shape::on_modifier_3          (Vector &pos, Canvas &canvas)
{
    return;
}

void Circle_shape::on_move                (Vector &pos, Canvas &canvas)
{
    if (!state_.pressed)
        return;
    
    M_render_texture *draw_texture = (M_render_texture *)widget_;
    assert (draw_texture);
    draw_texture->clear (Transparent);

    last_center_ = center_;
    Vector pos_offset = pos - center_;

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
        last_center_  += Vector (pos_offset.get_x (), 0);
    }
    if (pos_offset.get_y () < 0.0)
    {
        last_center_  += Vector (0, pos_offset.get_y ());
    }

    circle_.setOutlineThickness (std::min (DEFAULT_CIRCLE_THICKNESS, MIN_CIRCLE_THICKNESS + min / max));

    circle_.setPosition (last_center_);

    draw_texture->texture_.draw (circle_);
    draw_texture->texture_.display ();
}

void Circle_shape::on_confirm             (Vector &pos, Canvas &canvas)
{
    if (!state_.pressed)
        return;

    circle_.setPosition (last_center_ + Vector (canvas.draw_rect_.left, canvas.draw_rect_.top));
    canvas.canvas_texture.draw (circle_);
    canvas.canvas_texture.display ();

    state_.pressed = false;

    delete (M_render_texture *)widget_;
    widget_ = nullptr;
}

void Circle_shape::on_cancel              (Canvas &canvas)
{
    if (state_.pressed)
    {
        delete (M_render_texture *)widget_;
        widget_ = nullptr;
        state_.pressed = false;
    }
}