#ifndef TOOL_PALETTE_H
#define TOOL_PALETTE_H

class Button;
class Button_palette;
#include "../../../graphic_structures/vector/vector.h"
#include "../tools.h"
// #include "../../../button/button.h"
// #include "../../../button/palette/palette.h"

class ColorPalette 
{
    Color fg_color_ = Red;
    Color bg_color_ = White;
public:
    Color getFGColor () const {return fg_color_;}; 
    Color getBGColor () const {return bg_color_;}; 
    void setFGColor (Color fg_color) {fg_color_ = fg_color;};
    void setBGColor (Color bg_color) {bg_color_ = bg_color;};
};

class Tool_palette 
{
    M_vector<Tool *> palette_;
    Tool *cur_tool_ = nullptr;
    
    ColorPalette color_palette_;
    
public:
    Tool_palette ();
    Tool_palette (int size, ...); //TODO
    ~Tool_palette ();

    Tool *get_cur_tool () const {return cur_tool_;};
    Color get_fg_color () const {return color_palette_.getFGColor ();};
    Color get_bg_color () const {return color_palette_.getBGColor ();};

    void add_tool (Tool *tool);
    // void change_cur_tool (Tool *tool);
    // void change_cur_tool (size_t tool_idx);
    friend Button;
    friend Button_palette;
    friend bool color_button_run_fn (void *widget, void *args);
    friend bool tool_run_fn (void *widget, void *args);
};

#endif /* TOOL_PALETTE_H */