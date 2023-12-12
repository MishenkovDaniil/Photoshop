#include "curve_plot.h"

CurvePlot::CurvePlot (size_t width, size_t height) : 
    plot_img_ (new plug::Texture (width, height))
{
    assert (plot_img_);

    plug::Vec2d shift ((width - DEFAULT_CURVE_SIZE.x) / 2, (height - DEFAULT_CURVE_SIZE.y) / 2);
    converter = CoordConverter (plug::Vec2d (shift.x, shift.y + height_), width_, height_);

    plot_texture_.create (width, height); init_texture ();
};

void CurvePlot::create (size_t width, size_t height) 
{
    plot_img_ = new plug::Texture (width, height); 
    assert (plot_img_);

    plug::Vec2d shift ((width - DEFAULT_CURVE_SIZE.x) / 2, (height - DEFAULT_CURVE_SIZE.y) / 2);
    converter = CoordConverter (plug::Vec2d (shift.x, shift.y + height_), width_, height_);

    plot_texture_.create (width, height); 
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
    if (converter.getCoordPos (key_vertices_[idx].position).x == 0 ||
        converter.getCoordPos (key_vertices_[idx].position).x == width_)
    {
        vertex.position.x = key_vertices_[idx].position.x;
    }

    vertex.position.y = std::min (converter.getTexturePos (0, 0).y,       vertex.position.y);
    vertex.position.y = std::max (converter.getTexturePos (0, height_).y, vertex.position.y);

    key_vertices_[idx] = vertex;
    update_texture ();
}

void CurvePlot::update_texture ()
{
    plot_texture_.clear (plug::White);
    draw_neutral ();
    draw_coords ();

    size_t vertex_num = 2 * (key_vertices_.get_size () - 1);
    plug::VertexArray vertices (plug::Lines, vertex_num);
    
    for (int i = 0; i < vertex_num; ++i)
    {
        vertices[i] = key_vertices_[(i + 1) / 2];
    }

    plot_texture_.draw (vertices);
    
    CircleShape circle (4);
    circle.setFillColor (plug::Red);
    for (int i = 0; i < key_vertices_.get_size (); ++i)
    {
        circle.setPosition (key_vertices_[i].position - plug::Vec2d (4, 4));
        plot_texture_.draw (circle);
    }
    is_changed_texture = true;
}

void CurvePlot::draw_neutral ()
{
    draw_line (plug::Vec2d (0, 0), plug::Vec2d (width_, height_), plug::Color (150, 150, 150));
}

void CurvePlot::draw_coords ()
{
    draw_vector (plug::Vec2d (0, 0), plug::Vec2d (0, height_));
    draw_vector (plug::Vec2d (0, 0), plug::Vec2d (width_, 0));

    size_t x = 0;
    for (int i = 0; i < 4; ++i)
    {
        x += width_ / 4;
        draw_line (plug::Vec2d (x, 0), plug::Vec2d (x, height_), plug::Color (200, 200, 200));
    }

    size_t y = 0;
    for (int i = 0; i < 4; ++i)
    {
        y += height_ / 4;
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

    plot_texture_.draw (line);
    plot_texture_.draw (arrow_l);
    plot_texture_.draw (arrow_r);
}

void CurvePlot::draw_line (plug::Vec2d coord_start_, plug::Vec2d coord_end_, plug::Color color)
{
    plug::Vec2d texture_start  = converter.getTexturePos (coord_start_);
    plug::Vec2d texture_end    = converter.getTexturePos (coord_end_);
    
    plug::VertexArray line (plug::Lines, 2);
    
    line[0].color = line[1].color = color;

    line[0].position = texture_start;
    line[1].position = texture_end;

    plot_texture_.draw (line);
}

int CurvePlot::getValue (const int val) // x value
{
    if (is_changed_texture)
    {
        plot_texture_.getTexture (*plot_img_);
        is_changed_texture = false;
    }
    
    plug::Vec2d scale = getScale ();
    size_t texture_width = plot_texture_.getSize ().x;

    int x = val * scale.x;

    for (int y = 1; y < height_; ++y)
    {
        plug::Vec2d texture_pos = converter.getTexturePos (x, y);
        plug::Color color1 = plot_img_->data[(int)texture_pos.x + texture_width * (int)texture_pos.y];

        if (color1.r == 255 && color1.g == 0 && color1.b == 0)
        {
            return  (y + scale.y / 2) / scale.y;
        }
    }

    return 0;
}