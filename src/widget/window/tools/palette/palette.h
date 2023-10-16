#ifndef TOOL_PALETTE_H
#define TOOL_PALETTE_H

class Button;
class Button_palette;
#include "../../../../vector.h"
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
    Tool_palette (int size, ...);
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

//button args (Tool, palette) -->pressed-->palette.change_tool (tool);
//canvas    canvas-->if (palette.cur_tool)-->apply...

// class Palette_view : public Widget
// {
//     Tool_palette &tool_palette_;

// public: 
//     Palette_view (Tool_palette &tool_palette) : tool_palette_ (tool_palette) {};
//     ~Palette_view () {};

//     void render (sf::RenderTarget &target) = 0;
//     bool on_mouse_pressed  (Mouse_key mouse_key, Vector &pos) = 0;
//     bool on_mouse_released (Mouse_key mouse_key, Vector &pos) = 0;
//     bool on_mouse_moved    (Vector &new_pos) = 0;    /// x, y - absolute values 
//     bool on_keyboard_pressed  (Keyboard_key key) = 0;
//     bool on_keyboard_released (Keyboard_key key) = 0;
//     bool on_time (float delta_sec) = 0;
// };


#endif /* TOOL_PALETTE_H */