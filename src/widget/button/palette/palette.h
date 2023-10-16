#ifndef BUTTON_PALETTE_H
#define BUTTON_PALETTE_H

#include "../../../list.h"
#include "../../widget.h"
#include "../button.h"
#include "../../window/tools/palette/palette.h"
#include "../../window/master_window/master_window.h"
#include "../../window/canvas/canvas.h"

struct Pair
{
    void *arg_1 = nullptr;
    void *arg_2 = nullptr;

    Pair (void *arg1, void *arg2) : arg_1 (arg1), arg_2 (arg2) {};
    ~Pair () {};    
};

class Button_palette : public Menu
{
    Tool_palette *palette_;

public:
    Button_palette (Vector lh_pos, int width, int height, Tool_palette *palette);
    ~Button_palette ();

    void add_tool_button (Button *tool_button);
};

bool color_button_run_fn (void *widget, void *args);
bool tool_run_fn (void *widget, void *args);

#endif /* BUTTON_PALETTE_H */