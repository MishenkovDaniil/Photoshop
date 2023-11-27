#ifndef TOOL_PALETTE_H
#define TOOL_PALETTE_H

#include "../../../graphic_structures/vector/vector.h"
#include "../../../standard/plug_tool.h"

class Tool;
class Button;
class Button_palette;

class Tool_palette 
{
    M_vector<plug::Tool *> palette_;
    plug::Tool *cur_tool_ = nullptr;
    
    plug::ColorPalette color_palette_;
    
public:
    Tool_palette ();
    Tool_palette (int size, ...); //TODO
    ~Tool_palette ();

    plug::Tool *get_cur_tool () const {return cur_tool_;};
    plug::Color get_fg_color () const {return color_palette_.getFGColor ();};
    plug::Color get_bg_color () const {return color_palette_.getBGColor ();};

    void add_tool (plug::Tool *tool);
    // void change_cur_tool (Tool *tool);
    // void change_cur_tool (size_t tool_idx);
    friend Button;
    friend Button_palette;
    friend bool color_button_run_fn (void *widget, void *args);
    friend bool tool_run_fn (void *widget, void *args);
};

#endif /* TOOL_PALETTE_H */