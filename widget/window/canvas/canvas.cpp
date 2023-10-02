#include "canvas.h"


Canvas::Canvas (int width, int height, const Color color, const Vector lh_pos) :
    width_ (width),
    height_ (height),
    lh_pos_ (lh_pos), 
    color_ (color) 
{
    if (width <= 0 || height <= 0)
    {
        fprintf (stderr, "Error: incorrect size of canvas.\n");
        return;
    }
    canvas_texture.create (width, height);
};

Canvas::~Canvas () 
{
    width_  = __INT_MAX__;
    height_ = __INT_MAX__;
};

void Canvas::render (sf::RenderTarget &target) const
{
    sf::Sprite canvas_sprite (canvas_texture);
    target.draw (canvas_sprite);
}

bool Canvas::on_mouse_pressed  (Mouse_key mouse_key, Vector &pos)
{
    if (draw_tool.is_pressed)
    {
        switch (draw_tool.type)
        {
            case Brush:
            {
                on_brush (mouse_key, pos);
                return true;
            }
            default:
            {
                return false;
            }
        }
    }

    return false;
}

bool Canvas::on_mouse_released (Mouse_key mouse_key, Vector &pos)
{
    if (draw_tool.is_pressed)
    {
        switch (draw_tool.type)
        {
            case Brush:
            {
                return on_brush (mouse_key, pos);
            }
            default:
            {
                return false;
            }
        }
    }

    return false;
}

bool Canvas::on_mouse_moved    (Vector &new_pos)
{
    return false;
}   
 /// x, y - absolute values 
bool Canvas::on_keyboard_pressed  (Keyboard_key key)
{
    return false;
}

bool Canvas::on_keyboard_released (Keyboard_key key)
{
    return false;
}

bool Canvas::on_time (float delta_sec)
{
    // TODO
    return false;
}

bool Canvas::on_brush (Mouse_key mouse_key, Vector &pos)
{
    static sf::Uint8 pixel[4] = {0};
    if (mouse_key == Left)
    {
        if (draw_tool.is_pressed)
        {
            pixel[0] = color_.r_;
            pixel[1] = color_.g_;
            pixel[2] = color_.b_;
            pixel[3] = color_.a_;
        }
        else ///if (!draw_tool.is_pressed)
        {
            canvas_texture.update (pixel, 1, 1, pos.get_x (), pos.get_y ());
        }
        return true;
    }
    
    return false;
}