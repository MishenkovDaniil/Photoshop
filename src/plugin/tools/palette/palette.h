#ifndef TOOL_PALETTE_H
#define TOOL_PALETTE_H

#include "../../../graphic_structures/vector/vector.h"
#include "../../../standard/plug_tool.h"

class Tool;
class Button;
class Button_palette;

class ColorPalette : public plug::ColorPalette
{
        plug::Color fg_color_ = plug::Red;
        plug::Color bg_color_ = plug::White;
public:
        plug::Color getFGColor () const {return fg_color_;}; 
        plug::Color getBGColor () const {return bg_color_;}; 
        void setFGColor (plug::Color fg_color) {fg_color_ = fg_color;};
        void setBGColor (plug::Color bg_color) {bg_color_ = bg_color;};
};

class Tool_palette 
{
    M_vector<plug::Tool *> palette_;
    plug::Tool *cur_tool_ = nullptr;
    
    ColorPalette color_palette_;
    
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
    friend bool curves_func (void *widget, void *arg);
};

#endif /* TOOL_PALETTE_H */