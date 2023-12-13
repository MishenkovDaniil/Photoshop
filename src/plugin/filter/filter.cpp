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

void Curve_filter::applyFilter (plug::Canvas &canvas) const
{
    assert (curve_manage_button_);
    curve_manage_button_->set_arg (new std::pair<void *, void *>(&canvas, (void *)this));
    curve_manage_button_->run ();
}

void Curve_filter::applyCurveFilter (plug::Canvas &canvas, CurvePlot &plot) const
{  
    unsigned int width  = canvas.getSize ().x;
    unsigned int height = canvas.getSize ().y;
    
    const plug::Texture &texture_img = canvas.getTexture ();
    const plug::Texture &curve_texture = plot.getRenderTexture ().getTexture ();

    unsigned int curve_width  = plot.getSize ().x;
    unsigned int curve_height = plot.getSize ().y;
    
    plug::Vec2d curve_scale = plot.getScale ();

    int color_table[256];
    for (int i = 0; i < 256; ++i)
    {
        color_table[i] = -1;
    }
    for (int idx = 0; idx < width * height; ++idx)
    {
        plug::Color prev_color = canvas.getPixel (idx % width, idx / width);
        plug::Color new_color  = prev_color;
        int new_red = -1;
        
        if (color_table[prev_color.r] == -1)
        {
            new_red = color_table[prev_color.r] = plot.getValue (prev_color.r);
        }
        else
        {
            new_red = color_table[prev_color.r];
        }

        new_color.r = new_red;
        canvas.setPixel (idx % width, idx / width, new_color);
    }
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