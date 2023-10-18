#include "canvas.h"

Canvas::Canvas (int width, int height, const Color color, const Vector lh_pos, Tool_palette *palette) :
    transform_ (Transform (lh_pos)),
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

void Canvas::render (sf::RenderTarget &target, M_vector<Transform> &transform_stack)
{
    Transform top = transform_stack.get_size () > 0 ? transform_stack.top () : Transform (Vector (0, 0));
    Transform unite = transform_.unite (top);
    transform_stack.push (unite);

    Vector lh_pos = transform_stack.top ().offset_;

    sf::Sprite canvas_sprite (canvas_texture.getTexture ());
    canvas_sprite.setPosition (lh_pos);
    canvas_sprite.setTextureRect (draw_rect_);
     
    target.draw (canvas_sprite);
    
    if (palette_)
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

bool Canvas::on_mouse_pressed  (Mouse_key mouse_key, Vector &pos)
{
    Vector pos_ = transform_.apply_transform (pos);
    if (!contains (pos_.get_x (), pos_.get_y ()))
        return false;
    
    Vector texture_offset (draw_rect_.left, draw_rect_.top);
    pos_ += texture_offset;

    if (!palette_)
        return false;
    
    Tool *tool = palette_->get_cur_tool ();
    if (!tool)
        return false;
    
    Button_state state;
    state.pressed = true;
    state.released = false;

    tool->on_main_button (state, pos_, *this);
    return true;
}

bool Canvas::on_mouse_released (Mouse_key mouse_key, Vector &pos)
{
    Vector pos_ = transform_.apply_transform (pos);
    Vector texture_offset (draw_rect_.left, draw_rect_.top);
    pos_ += texture_offset;

    if (!palette_)
        return false;
    
    Tool *tool = palette_->get_cur_tool ();
    if (!tool)
        return false;
    
    Button_state state;
    state.pressed = false;
    state.released = true;

    tool->on_confirm (pos_, *this);
    return true;
}

bool Canvas::on_mouse_moved    (Vector &new_pos)
{
    Vector pos_ = transform_.apply_transform (new_pos);
    Vector texture_offset (draw_rect_.left, draw_rect_.top);
    pos_+= texture_offset;

    if (!palette_)
        return false;
    
    Tool *tool = palette_->get_cur_tool ();
    if (!tool)
        return false;
    
    tool->on_move (pos_, *this);
    return true;
}   

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

bool Canvas::on_brush (Mouse_key mouse_key, Vector &pos)
{
    Vector brush_pos;
    static sf::Vertex vertex[2];
    static int idx = 0;
    
    if ((!contains (pos.get_x (), pos.get_y ())) && !idx)
        return false;

    if (mouse_key == M_Left)
    {
        brush_pos = Vector (draw_rect_.left, draw_rect_.top) + pos;
        
        vertex[idx] = sf::Vertex (brush_pos, draw_tool.color);

        if (idx)
        {
            (vertex[0].position == vertex[1].position) ? canvas_texture.draw(&vertex[0], 1, sf::Points) :
                                                         canvas_texture.draw (vertex,    2, sf::Lines);
            canvas_texture.display ();
        }

        idx = !idx;

        return true;
    }
    
    return false;
}