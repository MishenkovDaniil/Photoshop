#include "palette.h"

plug::Button_palette::Button_palette (plug::Vec2d lh_pos, int width, int height, plug::Tool_palette *palette) : 
    plug::Menu (lh_pos, width, height),
    palette_ (palette)
{};

plug::Button_palette::~Button_palette () 
{
    size_t button_num = buttons.get_size ();
    for (int tool_button_idx = 0; tool_button_idx < button_num; ++tool_button_idx)
    {
        plug::Button *tool_button = buttons[tool_button_idx];
        if (!tool_button)
        {
            fprintf (stderr, "Event error: nil tool_button is detected.\n"
                             "Hint: nil tool_button idx in tool_buttons vector = %d.\n"
                             "Hint: tool_buttons vector size = %lu.\n", tool_button_idx, button_num);
            return;
        }
        assert (tool_button);

        delete (plug::Pair *)(tool_button->arg_);
    }
}

void plug::Button_palette::onKeyboardPressed  (const plug::KeyboardPressedEvent &event, plug::EHC &ehc)
{
    switch (event.key_id)
    {
        case B:
        {
            buttons[Brush_idx]->run ();
            ehc.stopped = true;
            return;
        }
        case L:
        {
            buttons[Line_idx]->run ();
            ehc.stopped = true;
            return;
        }
        case C:
        {
            buttons[Circle_shape_idx]->run ();
            ehc.stopped = true;
            return;
        }
        case R:
        {
            buttons[Rect_shape_idx]->run ();
            ehc.stopped = true;
            return;
        }
        case F:
        {
            buttons[Fill_idx]->run ();
            ehc.stopped = true;
            return;
        }
        case T:
        {
            buttons[Text_idx]->run ();
            ehc.stopped = true;
            return;
        }
        default:
        {
            break;
        }
    }
    // return false;
}

void plug::Button_palette::onKeyboardReleased (const plug::KeyboardReleasedEvent &event, plug::EHC &ehc)
{
    return;
}


void plug::Button_palette::add_tool_button (plug::Button *tool_button)
{
    assert (tool_button);

    plug::Pair *tool_button_args = new plug::Pair((void *)palette_, tool_button->arg_);

    tool_button->set_arg (tool_button_args);

    plug::Menu::add_button (tool_button);    
}

bool plug::color_button_run_fn (void *widget, void *args) 
{
    assert (widget && args);

    static bool is_init = false;
    plug::Master_window *m_window = (plug::Master_window *)widget;

    plug::Pair *args_ = (plug::Pair *)args;
    plug::Tool_palette *palette = (plug::Tool_palette *)args_->arg_1;
    plug::Color *color = (plug::Color *)args_->arg_2;
    assert (palette && color);
   
    if (!is_init)
    {
        init_canvases (m_window, palette);
        is_init = true;
    }
    
    palette->color_palette_.setFGColor (*color);
    
    return true;
}

bool plug::tool_run_fn (void *widget, void *args)
{
    static bool is_init = false;
    plug::Master_window *m_window = (plug::Master_window *)widget;

    plug::Pair *args_ = (plug::Pair *)args;

    plug::Tool_palette *palette = (plug::Tool_palette *)args_->arg_1;
    plug::Tool *new_active_tool = (plug::Tool *)args_->arg_2;
    assert (palette && new_active_tool);

    if (!is_init)
    {
        init_canvases (m_window, palette);
        is_init = true;
    }

    palette->cur_tool_ = new_active_tool;
    
    return true;
}

void plug::init_canvases (plug::Master_window *m_window, plug::Tool_palette *palette)
{
    for (int i = 0; i < m_window->windows.size; ++i)
    {
        plug::Window *window = (plug::Window *)list_get (&(m_window->windows), i + 1);
        if (window->canvas_->palette_ == palette)
            break;
        window->canvas_->palette_ = palette;
    }
}