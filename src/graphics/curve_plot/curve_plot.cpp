#include "curve_plot.h"

CurvePlot::CurvePlot (size_t width, size_t height) : 
    M_render_texture (width, height),
    plot_img_ (new plug::Texture (width, height))
{
    assert (plot_img_);

    plug::Vec2d shift ((width - DEFAULT_CURVE_SIZE.x) / 2, (height - DEFAULT_CURVE_SIZE.y) / 2);
    converter = CoordConverter (plug::Vec2d (shift.x, shift.y + height_), width_, height_);

    init_texture ();
};

void CurvePlot::create (size_t width, size_t height) 
{
    plot_img_ = new plug::Texture (width, height); 
    assert (plot_img_);

    plug::Vec2d shift ((width - DEFAULT_CURVE_SIZE.x) / 2, (height - DEFAULT_CURVE_SIZE.y) / 2);
    converter = CoordConverter (plug::Vec2d (shift.x, shift.y + height_), width_, height_);

    M_render_texture::create (width, height);
    init_texture ();
};

void CurvePlot::init_texture ()
{
    plug::VertexArray vertices (plug::Points, 3);
    vertices[0].position = converter.getTexturePos (0, 0);
    vertices[1].position = converter.getTexturePos (width_ / 2, height_ / 2);
    vertices[2].position = converter.getTexturePos (width_, height_);

    vertices[0].color = vertices[1].color = vertices[2].color = plug::Red;

    key_vertices_.add (vertices[0]);
    key_vertices_.add (vertices[1]);
    key_vertices_.add (vertices[2]);
    update_texture ();
}

int CurvePlot::contains (const plug::Vec2d &point)
{
    size_t size = key_vertices_.get_size ();
    for (size_t i = 0; i < size; ++i)
    {
        if (((int)key_vertices_[i].position.x <= (int)point.x + 4) && 
            ((int)key_vertices_[i].position.x >= (int)point.x - 4) &&
            ((int)key_vertices_[i].position.y <= (int)point.y + 4) &&
            ((int)key_vertices_[i].position.y >= (int)point.y - 4))
            return i;
    }

    return -1;
}

int CurvePlot::remove_key_point (size_t idx)
{
    size_t size = key_vertices_.get_size ();

    if (idx == 0 || idx == size - 1)
        return size;
    
    key_vertices_.erase (idx);

    update_texture ();
    
    return key_vertices_.get_size ();
}

int CurvePlot::add_key_point (plug::Vertex &vertex)
{
    size_t size = key_vertices_.get_size ();

    if (converter.getCoordPos (vertex.position).x < 0 ||
        converter.getCoordPos (vertex.position).x > width_ || 
        converter.getCoordPos (vertex.position).y < 0 ||
        converter.getCoordPos (vertex.position).y > height_
        )
        return size;
    
    for (size_t i = 0; i < size - 1; ++i)
    {
        if ((key_vertices_[i].position.x < vertex.position.x) && 
            (key_vertices_[i + 1].position.x > vertex.position.x))
        {
            key_vertices_.insert (i + 1, vertex);
            update_texture ();
            return i + 1;
        }
    }

    key_vertices_.add (vertex);
    update_texture ();
    return size;
}

void CurvePlot::change_key_point (size_t idx, plug::Vertex &vertex)
{
    if (idx == 0 || idx == key_vertices_.get_size () - 1)
    {
        vertex.position.x = key_vertices_[idx].position.x;
    }
    else if (vertex.position.x < key_vertices_[idx - 1].position.x)
    {  
        vertex.position.x = key_vertices_[idx - 1].position.x + 1;
    }
    else if (vertex.position.x > key_vertices_[idx + 1].position.x)
    {  
        vertex.position.x = key_vertices_[idx + 1].position.x - 1;
    }

    vertex.position.y = std::min (converter.getTexturePos (0, 0).y,       vertex.position.y);
    vertex.position.y = std::max (converter.getTexturePos (0, height_).y, vertex.position.y);

    key_vertices_[idx] = vertex;
    update_texture ();
}

void CurvePlot::update_texture ()
{
    clear (plug::White);
    draw_neutral ();
    draw_coords ();

    size_t vertex_num = key_vertices_.get_size ();

    double thickness = 2;
    plug::Vec2d offset (thickness, thickness);

    if (vertex_num == 2)
    {
        catmullRomLineDraw (key_vertices_[0].position - offset, 
                            key_vertices_[1].position - offset, 
                            thickness, *this, plug::Red);
    }
    else if (vertex_num == 3)
    {
        catmullRomLeftSplineDraw (  key_vertices_[0].position - offset, 
                                    key_vertices_[1].position - offset, 
                                    key_vertices_[2].position - offset, 
                                    thickness, *this, plug::Red);
        catmullRomRightSplineDraw ( key_vertices_[0].position - offset, 
                                    key_vertices_[1].position - offset, 
                                    key_vertices_[2].position - offset, 
                                    thickness, *this, plug::Red);
    }
    else
    {
        catmullRomLeftSplineDraw (  key_vertices_[0].position - offset, 
                                    key_vertices_[1].position - offset, 
                                    key_vertices_[2].position - offset, 
                                    thickness, *this, plug::Red);

        for (int i = 0; i < vertex_num - 3; ++i)
        {
            catmullRomCentralSplineDraw (   key_vertices_[i + 0].position - offset, 
                                            key_vertices_[i + 1].position - offset, 
                                            key_vertices_[i + 2].position - offset, 
                                            key_vertices_[i + 3].position - offset, 
                                            thickness, *this, plug::Red);
        }

        catmullRomRightSplineDraw ( key_vertices_[vertex_num - 3].position - offset, 
                                    key_vertices_[vertex_num - 2].position - offset, 
                                    key_vertices_[vertex_num - 1].position - offset, 
                                    thickness, *this, plug::Red);
    }
    
    CircleShape circle (4);
    circle.setFillColor (plug::Red);
    for (int i = 0; i < key_vertices_.get_size (); ++i)
    {
        circle.setPosition (key_vertices_[i].position - plug::Vec2d (4, 4));
        draw (circle);
    }
    is_changed_texture = true;
}

void CurvePlot::draw_neutral ()
{
    draw_line (plug::Vec2d (0, 0), plug::Vec2d (width_, height_), plug::Color (100, 100, 100));
}

void CurvePlot::draw_coords ()
{
    draw_vector (plug::Vec2d (0, 0), plug::Vec2d (0, height_));
    draw_vector (plug::Vec2d (0, 0), plug::Vec2d (width_, 0));

    size_t x = 0;
    for (int i = 0; i < line_num; ++i)
    {
        x += width_ / line_num;
        draw_line (plug::Vec2d (x, 0), plug::Vec2d (x, height_), plug::Color (200, 200, 200));
    }

    size_t y = 0;
    for (int i = 0; i < line_num; ++i)
    {
        y += height_ / line_num;
        draw_line (plug::Vec2d (0, y), plug::Vec2d (width_, y), plug::Color (200, 200, 200));
    }
}

void CurvePlot::draw_vector (plug::Vec2d coord_start_, plug::Vec2d coord_end_, plug::Color color)
{
    plug::Vec2d texture_start  = converter.getTexturePos (coord_start_);
    plug::Vec2d texture_end  = converter.getTexturePos (coord_end_);
    
    plug::VertexArray line (plug::Lines, 2);
    plug::VertexArray arrow_l (plug::Lines, 2);
    plug::VertexArray arrow_r (plug::Lines, 2);
    
    plug::Vec2d vec = coord_end_ - coord_start_;
    double len = vec.length ();
     
    plug::Vec2d normal  =  plug::normalize (plug::Vec2d (-vec.y, vec.x)) * 5;
    plug::Vec2d opposite = plug::normalize (-vec) * 10;
    plug::Vec2d arrow_l_vec = normal + opposite;
    plug::Vec2d arrow_r_vec = -normal + opposite;
    arrow_l_vec.y = -arrow_l_vec.y;
    arrow_r_vec.y = -arrow_r_vec.y;
    
    line[0].color = line[1].color = arrow_l[0].color = arrow_l[1].color = arrow_r[0].color = arrow_r[1].color = color;

    line[0].position = texture_start;
    line[1].position = arrow_l[0].position = arrow_r[0].position = texture_end;

    arrow_l[1].position = arrow_l[0].position + arrow_l_vec;
    arrow_r[1].position = arrow_r[0].position + arrow_r_vec;

    M_render_texture::draw (line);
    M_render_texture::draw (arrow_l);
    M_render_texture::draw (arrow_r);
}

void CurvePlot::draw_line (plug::Vec2d coord_start_, plug::Vec2d coord_end_, plug::Color color)
{
    plug::Vec2d texture_start  = converter.getTexturePos (coord_start_);
    plug::Vec2d texture_end    = converter.getTexturePos (coord_end_);
    
    plug::VertexArray line (plug::Lines, 2);
    
    line[0].color = line[1].color = color;

    line[0].position = texture_start;
    line[1].position = texture_end;

    M_render_texture::draw (line);
}

int CurvePlot::getValue (const int val) // x value
{
    if (is_changed_texture)
    {
        texture_.display ();
        texture_.getTexture (*plot_img_);
        is_changed_texture = false;
    }
    
    plug::Vec2d scale = getScale ();
    size_t texture_width = texture_.getSize ().x;

    int x = val * scale.x;

    for (int y = 1; y < height_; ++y)
    {
        plug::Vec2d texture_pos = converter.getTexturePos (x, y);
        plug::Color color1 = plot_img_->data[(int)texture_pos.x + texture_width * (int)texture_pos.y];

        if (color1 == plug::Red)
        {
            return  (y + scale.y / 2) / scale.y;
        }
    }

    return 0;
}

void CurvePlot::draw (const Drawable &drawable)
{   
    plug::Vec2d pos = drawable.getPosition ();
    plug::Vec2d start_pos = converter.getTexturePos(0, 0);
    plug::Vec2d end_pos = converter.getTexturePos(width_, height_);
    
    pos.x = std::max (pos.x, start_pos.x);
    pos.x = std::min (pos.x, end_pos.x);

    pos.y = std::max (pos.y, end_pos.y);
    pos.y = std::min (pos.y, start_pos.y);

    if (pos.x != drawable.getPosition ().x || 
        pos.y != drawable.getPosition ().y)
    
    {
        Drawable *new_drawable = drawable.clone ();
        new_drawable->setPosition (pos);
        M_render_texture::draw (*new_drawable);

        delete new_drawable;
    }
    else 
    {
        M_render_texture::draw (drawable);
    }
}

void CurvePlot::reset ()
{
    size_t vertex_num = key_vertices_.get_size ();
    for (int i = 0; i < vertex_num; ++i)
    {
        key_vertices_.pop ();
    }
    init_texture ();
}
