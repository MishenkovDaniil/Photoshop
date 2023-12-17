#ifndef CURVE_PLOT_H
#define CURVE_PLOT_H

#include <iostream>

#include "../../graphic_structures/vector/vector.h"
#include "../../graphic_structures/color/color.h"
#include "../rendertexture/rendertexture.h"
#include "../../standard/standard.h"
#include "../../vector.h"
#include "../../widget/texture_widget/texture_widget.h"
#include "../../catmullRom/catmull.h"

static const plug::Vec2d DEFAULT_CURVE_SIZE = plug::Vec2d (256 * 2, 256 * 2);

class CoordConverter 
{
    plug::Vec2d start_;
    size_t height_;
    size_t width_;

public:
    CoordConverter (plug::Vec2d start, size_t width, size_t height) : start_ (start), width_ (width), height_ (height) {};
    CoordConverter () = default;
    CoordConverter (const CoordConverter &other) = default;
    ~CoordConverter () = default;

    plug::Vec2d getTexturePos   (const plug::Vec2d &pos)    {return start_ + plug::Vec2d (pos.x, -pos.y);}; ///SFML-only
    plug::Vec2d getCoordPos     (const plug::Vec2d &pos)    {plug::Vec2d vec = pos - start_; return plug::Vec2d (vec.x, - vec.y);}; ///SFML-only
    plug::Vec2d getTexturePos   (int x, int y)              {return start_ + plug::Vec2d (x, -y);}; ///SFML-only
    plug::Vec2d getCoordPos     (int x, int y)              {return getCoordPos (plug::Vec2d (x, y));}; ///SFML-only
};

class CurvePlot : public M_render_texture
{
    size_t width_ = DEFAULT_CURVE_SIZE.x;
    size_t height_ = DEFAULT_CURVE_SIZE.y;
    size_t line_num = 8;
    
    M_vector<plug::Vertex> key_vertices_ = M_vector<plug::Vertex> (plug::Vertex ());    

    bool is_changed_texture = false;
    plug::Texture *plot_img_;
    CoordConverter converter;

public:
    CurvePlot (size_t width, size_t height);
    CurvePlot () = default;
    ~CurvePlot () = default;

    void create (size_t width, size_t height);
    
    const RenderTexture &getRenderTexture () {texture_.display (); return texture_;}; 
    plug::Vec2d getSize ()  {return plug::Vec2d (width_, height_);};
    plug::Vec2d getScale () {return plug::Vec2d ((double)width_ / 256.0, (double)height_ / 256.0);};
    int getValue (const int val);
    
    int contains (const plug::Vec2d &point);
    int remove_key_point (size_t idx);
    int add_key_point (plug::Vertex &vertex);
    void change_key_point (size_t idx, plug::Vertex &vertex);

private:
    void draw_coords ();
    void draw_neutral ();
    void init_texture ();
    void update_texture ();
    void draw_vector (plug::Vec2d coord_start_, plug::Vec2d coord_end_, plug::Color color = plug::Black);
    void draw_line (plug::Vec2d coord_start_, plug::Vec2d coord_end_, plug::Color color = plug::Black);
};

#endif /* CURVE_PLOT_H */