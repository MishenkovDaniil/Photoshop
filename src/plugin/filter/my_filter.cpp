#include "my_filter.h"

extern "C" plug::Plugin *loadPlugin (size_t type)
{
    return ((plug::PluginGuid)type == plug::PluginGuid::Filter) ? new White_black_filter : nullptr;
}

void White_black_filter::apply_filter (plug::Canvas &canvas) const
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
            double new_color_val = (LUMA_R_PARAM * prev_color.r + LUMA_G_PARAM * prev_color.g + LUMA_B_PARAM * prev_color.b);
            plug::Color new_color (new_color_val, new_color_val, new_color_val);
            canvas.setPixel (idx % width, idx / width, new_color);
        }
    }
}