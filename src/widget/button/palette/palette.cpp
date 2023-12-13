#include "palette.h"

Button_palette::Button_palette (plug::Vec2d lh_pos, int width, int height, Tool_palette *palette) : 
    Menu (lh_pos, width, height),
    palette_ (palette)
{};

Button_palette::~Button_palette () 
{
    size_t button_num = buttons.get_size ();
    for (int tool_button_idx = 0; tool_button_idx < button_num; ++tool_button_idx)
    {
        Button *tool_button = buttons.top ();
        if (!tool_button)
        {
            fprintf (stderr, "Event error: nil tool_button is detected.\n"
                             "Hint: nil tool_button idx in tool_buttons vector = %d.\n"
                             "Hint: tool_buttons vector size = %lu.\n", tool_button_idx, button_num);
            return;
        }
        assert (tool_button);

        Pair *button_args = (Pair *)tool_button->arg_;
        tool_button->arg_ = ((Pair *)tool_button->arg_)->arg_2;

        delete button_args;
        buttons.pop ();
    }
}

void Button_palette::onKeyboardPressed  (const plug::KeyboardPressedEvent &event, plug::EHC &ehc)
{
    switch (event.key_id)
    {
        case plug::KeyCode::B:
        {
            buttons[Brush_idx]->run ();
            ehc.stopped = true;
            return;
        }
        case plug::KeyCode::L:
        {
            buttons[Line_idx]->run ();
            ehc.stopped = true;
            return;
        }
        case plug::KeyCode::C:
        {
            buttons[Circle_shape_idx]->run ();
            ehc.stopped = true;
            return;
        }
        case plug::KeyCode::R:
        {
            buttons[Rect_shape_idx]->run ();
            ehc.stopped = true;
            return;
        }
        case plug::KeyCode::F:
        {
            buttons[Fill_idx]->run ();
            ehc.stopped = true;
            return;
        }
        case plug::KeyCode::T:
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

void Button_palette::onKeyboardReleased (const plug::KeyboardReleasedEvent &event, plug::EHC &ehc)
{
    return;
}

Button *Button_palette::popButton ()
{
    assert (buttons.get_size ());
    Button *tool_button = buttons.top ();
    assert (tool_button);

    Pair *button_args = (Pair *)tool_button->arg_;
    tool_button->arg_ = ((Pair *)tool_button->arg_)->arg_2;

    delete button_args;
    buttons.pop ();
    return tool_button;
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
    assert (widget && args);

    static bool is_init = false;
    Master_window *m_window = (Master_window *)widget;

    Pair *args_ = (Pair *)args;
    Tool_palette *palette = (Tool_palette *)args_->arg_1;
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

bool tool_run_fn (void *widget, void *args)
{
    static bool is_init = false;

    Master_window *m_window = (Master_window *)widget;

    Pair *args_ = (Pair *)args;

    Tool_palette *palette = (Tool_palette *)args_->arg_1;
    plug::Tool *new_active_tool = (plug::Tool *)args_->arg_2;
    assert (palette);
    assert (new_active_tool);

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