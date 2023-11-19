#ifndef TOOL_PALETTE_H
#define TOOL_PALETTE_H

class Button;
class Button_palette;
#include "../../../graphic_structures/vector/vector.h"
#include "../tools.h"
// #include "../../../button/button.h"
// #include "../../../button/palette/palette.h"

class Tool_palette 
{
    M_vector< Tool *> palette_;
    Tool *cur_tool_ = nullptr;
    
    Color fg_color_;
    Color bg_color_;
    
public:
    Tool_palette ();
    Tool_palette (int size, ...); //TODO
    ~Tool_palette ();

    Tool *get_cur_tool () const {return cur_tool_;};
    Color get_fg_color () const {return fg_color_;};
    Color get_bg_color () const {return bg_color_;};

    void add_tool (Tool *tool);
    // void change_cur_tool (Tool *tool);
    // void change_cur_tool (size_t tool_idx);
    friend Button;
    friend Button_palette;
    friend bool color_button_run_fn (void *widget, void *args);
    friend bool tool_run_fn (void *widget, void *args);
};

#endif /* TOOL_PALETTE_H */