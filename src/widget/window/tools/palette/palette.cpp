#include "palette.h"

Tool_palette::Tool_palette () :
    palette_ (M_vector ((Tool *)nullptr)) {};


Tool_palette::Tool_palette (int size, ...) :
    palette_ (M_vector ((Tool *)nullptr))
{
    ///TODO;
    ;
}

Tool_palette::~Tool_palette () {};

void Tool_palette::add_tool (Tool *tool)
{
    palette_.add (tool);
}