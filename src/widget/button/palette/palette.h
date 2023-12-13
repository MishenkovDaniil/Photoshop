#ifndef BUTTON_PALETTE_H
#define BUTTON_PALETTE_H

#include "../../../list.h"
#include "../../widget.h"
#include "../button.h"
#include "../../../plugin/tools/palette/palette.h"
#include "../../window/master_window/master_window.h"
#include "../../window/canvas/canvas.h"

struct Pair
{
    void *arg_1 = nullptr;
    void *arg_2 = nullptr;

    Pair (void *arg1, void *arg2) : arg_1 (arg1), arg_2 (arg2) {};
    ~Pair () {};    
};

enum Tool_buttons
{
    Brush_idx,
    Line_idx,
    Circle_shape_idx, 
    Rect_shape_idx,
    Fill_idx, 
    Text_idx
};

class Button_palette : public Menu
{
    Tool_palette *palette_;

public:
    Button_palette (plug::Vec2d lh_pos, int width, int height, Tool_palette *palette);
    ~Button_palette ();
    Button *popButton () override;

    void onKeyboardPressed  (const plug::KeyboardPressedEvent &event, plug::EHC &ehc) override;
    void onKeyboardReleased (const plug::KeyboardReleasedEvent &event, plug::EHC &ehc) override;
    void add_tool_button (Button *tool_button);
};

bool color_button_run_fn (void *widget, void *args);
bool tool_run_fn (void *widget, void *args);
void init_canvases (Master_window *m_window, Tool_palette *palette);

#endif /* BUTTON_PALETTE_H */