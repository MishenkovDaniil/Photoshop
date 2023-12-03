#include "my_filter.h"

extern "C" plug::Plugin *loadPlugin (size_t type)
{
    return ((plug::PluginGuid)type == plug::PluginGuid::Filter) ? new White_black_filter : nullptr;
}

void White_black_filter::applyFilter (plug::Canvas &canvas) const
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

MPluginData::MPluginData (const char *name, const char *texture_path)
{
    if (name)
    {
        name_ = new char[strlen (name) + 1];
        assert (name_ && "Error: failed to allocate memory for plugin name.\n");
        strcpy (name_, name);
    } 
    if (texture_path) 
    {
        path_ = new char[strlen (texture_path) + 1];
        assert (path_ && "Error: failed to allocate memory for plugin path.\n");
        strcpy (path_, texture_path);
    }
}

MPluginData::~MPluginData ()
{
    if (name_)
    {
        delete[] name_;
        name_ = nullptr;
    }
    if (path_)
    {
        delete[] path_;
        path_ = nullptr;
    }
}