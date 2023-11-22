#include "palette.h"

Tool_palette::Tool_palette () :
    palette_ (M_vector ((plug::Tool *)nullptr)), color_palette_ () {};


Tool_palette::Tool_palette (int size, ...) :
    palette_ (M_vector ((plug::Tool *)nullptr))
{
    ///TODO;
    ;
}

Tool_palette::~Tool_palette () {};

void Tool_palette::add_tool (plug::Tool *tool)
{
    palette_.add (tool);
    tool->setColorPalette (color_palette_);
    // if (palette_.get_size () == 1)cur_tool_ = palette_[0];
}