#include "filter.h"
#include "../../graphic_structures/color/hsl_luma_color.h"

plug::Filter *Filter_palette::get_last_filter () 
{

    return last_filter_;
}

void Filter_palette::set_last_filter (plug::Filter *filter) 
{
    last_filter_ = filter;
}   

plug::Filter *Filter_palette::get_filter      (size_t filter_idx) 
{
    return filters[filter_idx];
}

void Filter_palette::add_filter (plug::Filter *filter) 
{
    filters.add (filter);
} 

size_t Filter_palette::get_filter_count () 
{
    return filters.get_size ();
}   


void Light_filter::applyFilter (plug::Canvas &canvas) const
{
    const plug::SelectionMask *filter_mask = &(canvas.getSelectionMask ());

    unsigned int width  = canvas.getSize ().get_x ();
    unsigned int height = canvas.getSize ().get_y ();

    const plug::Texture &texture_img = canvas.getTexture ();

    for (int idx = 0; idx < width * height; ++idx)
    {
        if (filter_mask->getPixel(idx % width, idx / width))
        {
            plug::Color prev_color = canvas.getPixel (idx % width, idx / width);
            plug::Color new_color (prev_color.r, prev_color.g, prev_color.b);
            Luma_color luma_color = rgb_to_luma (new_color);
            luma_color.luma_ = std::max (MIN_LIGHT_VAL, std::min (luma_color.luma_ + (double)(delta_light_ / 100.0), 1.0));
            new_color = luma_to_rgb (luma_color);
            canvas.setPixel (idx % width, idx / width, new_color);
        }
    }
}

// void Saturation_filter::applyFilter (plug::Canvas &canvas) const
// {
//     const plug::SelectionMask *filter_mask = &(canvas.getSelectionMask ());

//     unsigned int width  = canvas.getSize ().get_x ();
//     unsigned int height = canvas.getSize ().get_y ();

//     const plug::Texture &texture_img = canvas.getTexture ();

//     for (int idx = 0; idx < width * height; ++idx)
//     {
//         if (filter_mask->getPixel(idx % width, idx / width))
//         {
//             plug::Color prev_color = canvas.getPixel (idx % width, idx / width);
//             plug::Color new_color (prev_color.r, prev_color.g, prev_color.b);
//             Hsl_color hsl_color = rgb_to_hsl (new_color);
//             hsl_color.saturation_ = std::max (MIN_SATURATION_VAL, std::min (hsl_color.saturation_ + (double)(delta_saturation_ / 100.0), 1.0));
//             new_color = hsl_to_rgb (hsl_color);
//             // printf ("new_color = ")
//             canvas.setPixel (idx % width, idx / width, new_color);
//         }
//     }
// }
Curve_filter::Curve_filter (Curve_func func, Master_window &window) : 
    Filter ( "curves"),
    curve_manage_button_ (new Button (plug::Vec2d (), 0, 0, func, &window, nullptr, plug::Transparent)) {};

Curve_filter::~Curve_filter () 
{
    if (curve_manage_button_) 
        delete curve_manage_button_; 
    curve_manage_button_ = nullptr;
};

void Curve_filter::applyFilter (plug::Canvas &canvas) const
{
    assert (curve_manage_button_);
    curve_manage_button_->set_arg (new std::pair<void *, void *>(&canvas, (void *)this));
    curve_manage_button_->run ();
}

void Curve_filter::applyCurveFilter (plug::Canvas &canvas, CurvePlot &plot) const
{  
    // unsigned int canvas_width  = canvas.getSize ().x;
    // unsigned int canvas_height = canvas.getSize ().y;
    // printf ("canvas_width = %u, canvas_height = %u\n", canvas_width, canvas_height);
    
    const plug::Texture &texture_img = canvas.getTexture ();
    const plug::Texture &curve_texture = plot.getTexture ();

    unsigned int width  = texture_img.width;
    unsigned int height = texture_img.height;
    
    // printf ("width = %u, height = %u\n", width, height);

    IntRect prev_rect = ((Canvas &)canvas).getDrawRect ();

    ((Canvas &)canvas).setDrawRectOffset (0, 0);

    unsigned int curve_width  = plot.getSize ().x;
    unsigned int curve_height = plot.getSize ().y;
    
    plug::Vec2d curve_scale = plot.getScale ();

    int r_color_table[256];
    int g_color_table[256];
    int b_color_table[256];
    
    for (int i = 0; i < 256; ++i)
    {
        r_color_table[i] = g_color_table[i] = b_color_table[i] = -1;
    }

    for (int idx = 0; idx < width * height; ++idx)
    {
        plug::Color prev_color = canvas.getPixel (idx % width, idx / width);
        
        uint8_t new_red     = find_component (prev_color.r, r_color_table, plot);
        uint8_t new_green   = find_component (prev_color.g, g_color_table, plot);
        uint8_t new_blue    = find_component (prev_color.b, b_color_table, plot);

        plug::Color new_color  (new_red, new_green, new_blue);

        canvas.setPixel (idx % width, idx / width, new_color);
    }

    ((Canvas &)canvas).setDrawRectOffset (prev_rect.getLeftCorner (), prev_rect.getTopCorner ());
}

int Curve_filter::find_component (uint8_t &val, int component[256], CurvePlot &plot) const
{
    if (component[val] == -1)
    {
        return component[val] = plot.getValue (val);
    }

    return component[val];
}

void White_black_filter::applyFilter (plug::Canvas &canvas) const
{
    const plug::SelectionMask &filter_mask = canvas.getSelectionMask ();

    unsigned int width  = canvas.getSize ().get_x ();
    unsigned int height = canvas.getSize ().get_y ();

    const plug::Texture &texture_img = canvas.getTexture ();

    for (int idx = 0; idx < width * height; ++idx)
    {
        if (filter_mask.getPixel(idx % width, idx / width))
        {
            plug::Color prev_color = canvas.getPixel (idx % width, idx / width);
            double new_color_val = (LUMA_R_PARAM * prev_color.r + LUMA_G_PARAM * prev_color.g + LUMA_B_PARAM * prev_color.b);
            plug::Color new_color (new_color_val, new_color_val, new_color_val);
            canvas.setPixel (idx % width, idx / width, new_color);
        }
    }
}