#include "palette.h"

plug::Tool_palette::Tool_palette () :
    palette_ (M_vector ((plug::Tool *)nullptr)), color_palette_ () {};


plug::Tool_palette::Tool_palette (int size, ...) :
    palette_ (M_vector ((plug::Tool *)nullptr))
{
    ///TODO;
    ;
}

plug::Tool_palette::~Tool_palette () {};

void plug::Tool_palette::add_tool (plug::Tool *tool)
{
    palette_.add (tool);
    tool->setColorPalette (color_palette_);
    // if (palette_.get_size () == 1)cur_tool_ = palette_[0];
}