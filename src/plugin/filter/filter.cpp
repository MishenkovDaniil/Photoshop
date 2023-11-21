#include "filter.h"

plug::Filter *plug::Filter_palette::get_last_filter () 
{
    return last_filter_;
}

void plug::Filter_palette::set_last_filter (plug::Filter *filter) 
{
    last_filter_ = filter;
}   

plug::Filter *plug::Filter_palette::get_filter      (size_t filter_idx) 
{
    return filters[filter_idx];
}

void plug::Filter_palette::add_filter (plug::Filter *filter) 
{
    filters.add (filter);
} 

size_t plug::Filter_palette::get_filter_count () 
{
    return filters.get_size ();
}   


void plug::Light_filter::apply_filter (plug::Canvas &canvas) const
{
    const plug::SelectionMask *filter_mask = &(canvas.getSelectionMask ());

    sf::Texture texture = canvas.canvas_texture.getTexture ();
    unsigned int width  = canvas.get_size ().get_x ();
    unsigned int height = canvas.get_size ().get_y ();

    sf::Image texture_img = texture.copyToImage ();

    sf::Uint8 *pixels = (sf::Uint8 *)calloc (width * height * 4, sizeof (sf::Uint8));
    assert (pixels);
    
    sf::IntRect rect = canvas.get_draw_rect ();

    for (int idx = 0; idx < width * height; ++idx)
    {
        if (filter_mask->get_pixel(idx % width, idx / width))
        {
            sf::Color prev_color = texture_img.getPixel (rect.left + idx % width, rect.top + idx / width);
            plug::Color new_color (prev_color.r, prev_color.g, prev_color.b);
            plug::Luma_color luma_color = rgb_to_luma (new_color);
            luma_color.luma_ = std::max (MIN_LIGHT_VAL, std::min (luma_color.luma_ + (double)(delta_light_ / 100.0), 1.0));
            new_color = luma_to_rgb (luma_color);
            ((plug::Color *)pixels)[idx] = new_color;
        }
    }

    texture.update (pixels, width, height, rect.left, rect.top);
    sf::Sprite sprite;
    sprite.setTexture (texture);

    canvas.canvas_texture.draw (sprite);
    canvas.canvas_texture.display ();

    free (pixels);
}

void plug::Saturation_filter::apply_filter (plug::Canvas &canvas) const
{
    const plug::SelectionMask *filter_mask = &(canvas.getSelectionMask ());
    sf::Texture texture = canvas.canvas_texture.getTexture ();
    unsigned int width  = canvas.get_size ().get_x ();
    unsigned int height = canvas.get_size ().get_y ();
    
    sf::Image texture_img = texture.copyToImage ();

    sf::Uint8 *pixels = (sf::Uint8 *)calloc (width * height * 4, sizeof (sf::Uint8));
    assert (pixels);
    
    sf::IntRect rect = canvas.get_draw_rect ();

    for (int idx = 0; idx < width * height; ++idx)
    {
        if (filter_mask->get_pixel(idx % width, idx / width))
        {
            sf::Color prev_color = texture_img.getPixel (rect.left + idx % width, rect.top + idx / width);
            plug::Color new_color (prev_color.r, prev_color.g, prev_color.b);
            plug::Hsl_color hsl_color = rgb_to_hsl (new_color);
            hsl_color.saturation_ = std::max (MIN_SATURATION_VAL, std::min (hsl_color.saturation_ + (double)(delta_saturation_ / 100.0), 1.0));
            new_color = hsl_to_rgb (hsl_color);
            ((plug::Color *)pixels)[idx] = new_color;
        }
    }

    texture.update (pixels, width, height, rect.left, rect.top);
    sf::Sprite sprite;
    sprite.setTexture (texture);

    canvas.canvas_texture.draw (sprite);
    canvas.canvas_texture.display ();

    free (pixels);
}

void plug::White_black_filter::apply_filter (plug::Canvas &canvas) const
{
    const plug::SelectionMask *filter_mask = &(canvas.getSelectionMask ());

    sf::Texture texture = canvas.canvas_texture.getTexture ();
    unsigned int width  = canvas.get_size ().get_x ();
    unsigned int height = canvas.get_size ().get_y ();
    
    sf::Image texture_img = texture.copyToImage ();

    sf::Uint8 *pixels = (sf::Uint8 *)calloc (width * height * 4, sizeof (sf::Uint8));
    assert (pixels);
    
    sf::IntRect rect = canvas.get_draw_rect ();

    for (int idx = 0; idx < width * height; ++idx)
    {
        if (filter_mask->get_pixel(idx % width, idx / width))
        {
            sf::Color prev_color = texture_img.getPixel (rect.left + idx % width, rect.top + idx / width);
            double new_color_val = (LUMA_R_PARAM * prev_color.r + LUMA_G_PARAM * prev_color.g + LUMA_B_PARAM * prev_color.b);
            plug::Color new_color (new_color_val, new_color_val, new_color_val);
            ((plug::Color *)pixels)[idx] = new_color;
        }
    }

    texture.update (pixels, width, height, rect.left, rect.top);
    sf::Sprite sprite;
    sprite.setTexture (texture);

    canvas.canvas_texture.draw (sprite);
    canvas.canvas_texture.display ();

    free (pixels);
}