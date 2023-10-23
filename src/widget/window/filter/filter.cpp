#include "filter.h"

bool Filter_mask::get_pixel (size_t x, size_t y)
{
    assert (mask && "nullptr filter mask\n");
    assert ((x < width_) && (y < height_) && "invalid pixel coord\n");

    return mask[x + y * width_];
}  

void Filter_mask::set_pixel (size_t x, size_t y, bool flag)
{
    assert (mask && "nullptr filter mask\n");
    assert ((x < width_) && (y < height_) && "invalid pixel coord\n");

    mask[x + y * width_] = flag;
}  

void Filter_mask::fill (bool value)
{
    assert (mask && "nullptr filter mask\n");
    for (size_t idx = 0; idx < width_ * height_; ++idx)
    {
        mask[idx] = value;
    }
}

Filter *Filter_palette::get_last_filter () 
{
    return last_filter_;
}

void    Filter_palette::set_last_filter (Filter *filter) 
{
    last_filter_ = filter;
}   

Filter *Filter_palette::get_filter      (size_t filter_idx) 
{
    return filters[filter_idx];
}

void    Filter_palette::add_filter (Filter *filter) 
{
    filters.add (filter);
} 

size_t  Filter_palette::get_filter_count () 
{
    return filters.get_size ();
}   


void Light_filter::apply_filter (Canvas &canvas, Filter_mask *mask) const
{
    assert (mask);

    sf::Texture texture = canvas.canvas_texture.getTexture ();
    unsigned int width  = texture.getSize ().x;
    unsigned int height = texture.getSize ().y;
    
    sf::Image texture_img = texture.copyToImage ();

    sf::Uint8 *pixels = (sf::Uint8 *)calloc (width * height * 4, sizeof (sf::Uint8));
    assert (pixels);

    for (int idx = 0; idx < width * height; ++idx)
    {
        if (mask->get_pixel(idx % width, idx / width))
        {
            sf::Color prev_color = texture_img.getPixel (idx % width, idx / width);
            prev_color.r = std::max (std::min ((int)prev_color.r + delta_light_, 0xff), 0);
            prev_color.g = std::max (std::min ((int)prev_color.g + delta_light_, 0xff), 0);
            prev_color.b = std::max (std::min ((int)prev_color.b + delta_light_, 0xff), 0);
            
            ((Color *)pixels)[idx] = prev_color;
        }
    }

    texture.update (pixels);
    sf::Sprite sprite;
    sprite.setTexture (texture);

    canvas.canvas_texture.draw (sprite);
    canvas.canvas_texture.display ();

    free (pixels);
}

