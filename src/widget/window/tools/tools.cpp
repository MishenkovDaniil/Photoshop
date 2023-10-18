#include "tools.h"
#include "../../texture_widget/texture_widget.h"

Brush::Brush () 
{};

Brush::~Brush () 
{};

void Brush::on_main_button         (Button_state &state, Vector &pos, Canvas &canvas)
{
    if (state.pressed)
    {
        pos += Vector (canvas.draw_rect_.left, canvas.draw_rect_.top);
        sf::CircleShape circle (10);
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

void Brush::on_modifier_1          (Button_state &state, Vector &pos, Canvas &canvas)
{
    return;
}

void Brush::on_modifier_2          (Button_state &state, Vector &pos, Canvas &canvas)
{
    return;
}

void Brush::on_modifier_3          (Button_state &state, Vector &pos, Canvas &canvas)
{
    return;
}

void Brush::on_move                (Vector &pos, Canvas &canvas)
{
    if (!is_pressed)
        return;

    pos += Vector (canvas.draw_rect_.left, canvas.draw_rect_.top);

    sf::CircleShape circle (10);
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

void Brush::on_cancel              (Vector &pos, Canvas &canvas)
{

    sf::CircleShape circle (10);
    circle.setPosition (prev_pos); //or pos maybe
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
        widget_ = new M_render_texture (canvas_size.get_x (), canvas_size.get_y (), Color (0, 0, 0, 0));
        assert (widget_);

        vertex[0] = sf::Vertex (pos, canvas.get_fg_color ());
        M_render_texture *draw_texture = (M_render_texture *)widget_;

        draw_texture->texture_.draw (vertex, 1, sf::Points);
        draw_texture->texture_.display ();
        vertex_idx = 1;
    }
}

void Line::on_secondary_button    (Button_state &state, Vector &pos, Canvas &canvas)
{
    return;   
}

void Line::on_modifier_1          (Button_state &state, Vector &pos, Canvas &canvas)
{
    return;   
}

void Line::on_modifier_2          (Button_state &state, Vector &pos, Canvas &canvas)
{
    return;   
}

void Line::on_modifier_3          (Button_state &state, Vector &pos, Canvas &canvas)
{
    return;   
}

void Line::on_move                (Vector &pos, Canvas &canvas)
{
    if (!vertex_idx)
        return;
    M_render_texture *draw_texture = (M_render_texture *)widget_;
    assert (draw_texture);
    draw_texture->clear (Color (0, 0, 0, 0));
    
    vertex[1] = sf::Vertex (pos, canvas.get_fg_color ());
    (vertex[0].position == vertex[1].position) ? draw_texture->texture_.draw(&vertex[0], 1, sf::Points) :
                                                 draw_texture->texture_.draw (vertex,    2, sf::Lines);
    draw_texture->texture_.display ();

    return;   
}

void Line::on_confirm             (Vector &pos, Canvas &canvas)
{
    if (!vertex_idx)
        return;

    Vector draw_rect_offset (canvas.draw_rect_.left, canvas.draw_rect_.top);

    vertex[1] = sf::Vertex (pos + draw_rect_offset, canvas.get_fg_color ());
    vertex[0].position += sf::Vector2f (draw_rect_offset.get_x (), draw_rect_offset.get_y ());

    (vertex[0].position == vertex[1].position) ? canvas.canvas_texture.draw(&vertex[0], 1, sf::Points) :
                                                 canvas.canvas_texture.draw (vertex,    2, sf::Lines);
    canvas.canvas_texture.display ();

    vertex_idx = 0;

    delete (M_render_texture *)widget_;
    widget_ = nullptr;

    return;   
}

void Line::on_cancel              (Vector &pos, Canvas &canvas)
{
    if (vertex_idx)
    {
        delete (M_render_texture *)widget_;
        widget_ = nullptr;
    }
    
    return;   
}

// bool Circle_shape_tool::apply_begin (sf::RenderTexture texture, Vector &pos)
// {
//     centre_pos = pos;
// }

// bool Circle_shape_tool::apply_continue (sf::RenderTexture texture, Vector &pos)
// {
//     sf::circleshape circle(centre_pos)
//     circle.radius = ro (pos - center_pos);
//     buffer_texture.draw (circle);
// }

// bool Circle_shape_tool::apply_end      (sf::RenderTexture texture, Vector &pos)
// {
//     apply_continue (texture, pos);
// }

// bool Circle_shape_tool::apply_cancel      (sf::RenderTexture texture, Vector &pos)
// {
//     buffer.clear (0, 0,0 ,0);
// }




// bool Rect_shape_tool::apply_begin (sf::RenderTexture texture, Vector &pos)
// {
//     lh_pos = pos;
// }

// bool Rect_shape_tool::apply_continue (sf::RenderTexture texture, Vector &pos)
// {
//     width = abs (pos.x - lh_pos.x);
//     height = abs (pos.y - lh_pos.y);
    
//     sf::rectshape rect(Vector (width, height));
//     buffer_texture.draw (rect);
// }

// bool Rect_shape_tool::apply_end      (sf::RenderTexture texture, Vector &pos)
// {
//     apply_continue (texture, pos);
// }

// bool Rect_shape_tool::apply_cancel      (sf::RenderTexture texture, Vector &pos)
// {
//     buffer.clear (0, 0,0 ,0);
// }