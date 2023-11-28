#include "filter.h"

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


void Light_filter::apply_filter (plug::Canvas &canvas) const
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

void Saturation_filter::apply_filter (plug::Canvas &canvas) const
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
            Hsl_color hsl_color = rgb_to_hsl (new_color);
            hsl_color.saturation_ = std::max (MIN_SATURATION_VAL, std::min (hsl_color.saturation_ + (double)(delta_saturation_ / 100.0), 1.0));
            new_color = hsl_to_rgb (hsl_color);
            canvas.setPixel (idx % width, idx / width, new_color);
        }
    }
}

// void White_black_filter::apply_filter (plug::Canvas &canvas) const
// {
//     const SelectionMask *filter_mask = &(canvas.getSelectionMask ());

//     unsigned int width  = canvas.getSize ().get_x ();
//     unsigned int height = canvas.getSize ().get_y ();

//     const plug::Texture &texture_img = canvas.getTexture ();

//     for (int idx = 0; idx < width * height; ++idx)
//     {
//         if (filter_mask->get_pixel(idx % width, idx / width))
//         {
//             plug::Color prev_color = canvas.getPixel (idx % width, idx / width);
//             double new_color_val = (LUMA_R_PARAM * prev_color.r + LUMA_G_PARAM * prev_color.g + LUMA_B_PARAM * prev_color.b);
//             plug::Color new_color (new_color_val, new_color_val, new_color_val);
//             canvas.setPixel (idx % width, idx / width, new_color);
//         }
//     }
// }