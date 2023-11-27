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
    printf ("start\n");
    const SelectionMask *filter_mask = &(canvas.getSelectionMask ());

    unsigned int width  = canvas.getSize ().get_x ();
    unsigned int height = canvas.getSize ().get_y ();

    const plug::Texture &texture_img = canvas.getTexture ();

    printf ("pixel[0][0] = %d %d %d %d\n", canvas.getPixel (0, 0).r, canvas.getPixel (0, 0).g, canvas.getPixel (0, 0).b, canvas.getPixel (0, 0).a);


    for (int idx = 0; idx < width * height; ++idx)
    {
        if (filter_mask->get_pixel(idx % width, idx / width))
        {
            plug::Color prev_color = canvas.getPixel (idx % width, idx / width);
            plug::Color new_color (prev_color.r, prev_color.g, prev_color.b);
            Luma_color luma_color = rgb_to_luma (new_color);
            luma_color.luma_ = std::max (MIN_LIGHT_VAL, std::min (luma_color.luma_ + (double)(delta_light_ / 100.0), 1.0));
            new_color = luma_to_rgb (luma_color);
            // ((plug::Color *)pixels)[idx] = new_color;
            canvas.setPixel (idx % width, idx / width, new_color);
        }
    }


    // plug::VertexArray vertices (plug::Quads, 4);

    // vertices[0].position = plug::Vec2d (0, 0);
    // vertices[1].position = plug::Vec2d (width, 0);
    // vertices[2].position = plug::Vec2d (width, height);
    // vertices[3].position = plug::Vec2d (0, height);
    // // Sprite sprite (canvas.getTexture ());
    // vertices[0].color = canvas.getPixel (0, 0);
    // vertices[1].color = canvas.getPixel (width - 1, 0);
    // vertices[2].color = canvas.getPixel (width - 1, height - 1);
    // vertices[3].color = canvas.getPixel (0, height - 1);
    // for (int i = 0; i < 4; ++i)
    // {
    //     printf ("vertex color = %d %d %d %d\n", vertices[i].color.r, vertices[i].color.g, vertices[i].color.b, vertices[i].color.a);
    // }

    // vertices[0].tex_coords = plug::Vec2d (0, 0);
    // vertices[1].tex_coords = plug::Vec2d (width - 1, 0);
    // vertices[2].tex_coords = plug::Vec2d (width - 1, height - 1);
    // vertices[3].tex_coords = plug::Vec2d (0, height - 1);

    // canvas.draw (vertices, canvas.getTexture ());
    // canvas.getRenderTexture ().display ();

    // free (pixels);
}

void Saturation_filter::apply_filter (plug::Canvas &canvas) const
{
    const SelectionMask *filter_mask = &(canvas.getSelectionMask ());

    unsigned int width  = canvas.getSize ().get_x ();
    unsigned int height = canvas.getSize ().get_y ();

    const plug::Texture &texture_img = canvas.getTexture ();

    for (int idx = 0; idx < width * height; ++idx)
    {
        if (filter_mask->get_pixel(idx % width, idx / width))
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

void White_black_filter::apply_filter (plug::Canvas &canvas) const
{
    const SelectionMask *filter_mask = &(canvas.getSelectionMask ());

    unsigned int width  = canvas.getSize ().get_x ();
    unsigned int height = canvas.getSize ().get_y ();

    const plug::Texture &texture_img = canvas.getTexture ();

    for (int idx = 0; idx < width * height; ++idx)
    {
        if (filter_mask->get_pixel(idx % width, idx / width))
        {
            plug::Color prev_color = canvas.getPixel (idx % width, idx / width);
            double new_color_val = (LUMA_R_PARAM * prev_color.r + LUMA_G_PARAM * prev_color.g + LUMA_B_PARAM * prev_color.b);
            plug::Color new_color (new_color_val, new_color_val, new_color_val);
            canvas.setPixel (idx % width, idx / width, new_color);
        }
    }
}