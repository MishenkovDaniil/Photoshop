#include "tools.h"


Brush::Brush () 
{};

Brush::~Brush () 
{};

void Brush::on_main_button         (Button_state &state, Vector &pos, Canvas &canvas)
{
    if (state.pressed)
    {
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