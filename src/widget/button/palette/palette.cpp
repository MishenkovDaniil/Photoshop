#include "palette.h"

Button_palette::Button_palette (Vector lh_pos, int width, int height, Tool_palette *palette) : 
    Menu (lh_pos, width, height),
    palette_ (palette)
{};

Button_palette::~Button_palette () 
{
    for (int tool_button_idx = 0; tool_button_idx < buttons.size; ++tool_button_idx)
    {
        Button *tool_button = (Button *)list_get (&buttons, tool_button_idx + 1);
        if (!tool_button)
        {
            fprintf (stderr, "Event error: nil tool_button is detected.\n"
                             "Hint: nil tool_button idx in tool_buttons list = %d.\n"
                             "Hint: tool_buttons list size = %d.\n", tool_button_idx + 1, buttons.size);
            return;
        }
        assert (tool_button);

        delete (Pair *)tool_button->arg_;
    }
}

void Button_palette::add_tool_button (Button *tool_button)
{
    assert (tool_button);

    Pair *tool_button_args = new Pair((void *)palette_, tool_button->arg_);

    tool_button->set_arg (tool_button_args);

    Menu::add_button (tool_button);    
}

bool color_button_run_fn (void *widget, void *args) 
{
    static bool is_init = false;
    Master_window *m_window = (Master_window *)widget;


    Pair *args_ = (Pair *)args;
    Tool_palette *palette = (Tool_palette *)args_->arg_1;
    Color *color = (Color *)args_->arg_2;
    assert (palette && color);
   
    if (!is_init)
    {
        init_canvases (m_window, palette);
        is_init = true;
    }
    
    palette->fg_color_ = *color;
    
    return true;
}

bool tool_run_fn (void *widget, void *args)
{
    static bool is_init = false;
    Master_window *m_window = (Master_window *)widget;

    Pair *args_ = (Pair *)args;

    Tool_palette *palette = (Tool_palette *)args_->arg_1;
    Tool *new_active_tool = (Tool *)args_->arg_2;
    assert (palette && new_active_tool);

    if (!is_init)
    {
        init_canvases (m_window, palette);
        is_init = true;
    }

    palette->cur_tool_ = new_active_tool;
    
    return true;
}

void init_canvases (Master_window *m_window, Tool_palette *palette)
{
    for (int i = 0; i < m_window->windows.size; ++i)
    {
        Window *window = (Window *)list_get (&(m_window->windows), i + 1);
        if (window->canvas_->palette_ == palette)
            break;
        window->canvas_->palette_ = palette;
    }
}