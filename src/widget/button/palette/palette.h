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
    Button_palette (Vec2d lh_pos, int width, int height, Tool_palette *palette);
    ~Button_palette ();

    void onKeyboardPressed  (const KeyboardPressedEvent &event, EHC &ehc) override;
    void onKeyboardReleased (const KeyboardReleasedEvent &event, EHC &ehc) override;
    void add_tool_button (Button *tool_button);
};

bool color_button_run_fn (void *widget, void *args);
bool tool_run_fn (void *widget, void *args);

#endif /* BUTTON_PALETTE_H */