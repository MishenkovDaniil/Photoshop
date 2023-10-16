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

// void Tool_palette::change_cur_tool (Tool *tool)
// {
//     cur_tool_ = tool;               /// ?? check in vector for contains ()
// }

// void Tool_palette::change_cur_tool (size_t tool_idx)
// {
//     cur_tool_ = palette_[tool_idx];
// }