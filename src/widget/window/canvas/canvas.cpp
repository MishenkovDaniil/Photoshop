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
};

void Canvas::render (sf::RenderTarget &target)
{
    sf::Sprite canvas_sprite (canvas_texture.getTexture ());
    canvas_sprite.setPosition (lh_pos_);
    canvas_sprite.setTextureRect (draw_rect_);
     
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

bool Canvas::contains (int x, int y)
{
    return ((x > lh_pos_.get_x ()) && 
            (x < lh_pos_.get_x ()  + width_) && 
            (y > lh_pos_.get_y ()) &&
            (y < lh_pos_.get_y () + height_));
}

bool Canvas::on_brush (Mouse_key mouse_key, Vector &pos)
{
    Vector brush_pos;
    static sf::Vertex vertex[2];
    static int idx = 0;
    
    if ((!contains (pos.get_x (), pos.get_y ())) && !idx)
        return false;

    if (mouse_key == M_Left)
    {
        brush_pos = Vector (draw_rect_.left, draw_rect_.top) + pos - lh_pos_;
        
        vertex[idx] = sf::Vertex (brush_pos, draw_tool.color);

        if (idx)
        {
            (vertex[0].position == vertex[1].position) ? canvas_texture.draw(&vertex[0], 1, sf::Points) :
                                                            canvas_texture.draw (vertex,   2, sf::Lines);
            canvas_texture.display ();
        }

        idx = !idx;

        return true;
    }
    
    return false;
}