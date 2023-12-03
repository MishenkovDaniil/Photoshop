#ifndef MASTER_WINDOW_H
#define MASTER_WINDOW_H

#include "../../../list.h"
#include "../menu/menu.h"
#include "../canvas/canvas.h"
#include "../window.h"
#include "../../../plugin/tools/palette/palette.h"

static int INIT_WINDOWS_CAPACITY = 10;

class Tool_palette;

class Master_window : public Window
{
    List windows = {};
    Menu *menu_ = nullptr;

    size_t first_ = 1;
    size_t last_  = 0;

public:
    Master_window (int width, int height, plug::Vec2d lh_pos, const char *w_name, int list_capacity = INIT_WINDOWS_CAPACITY);
    ~Master_window ();

    void add_menu_button (Button *button);
    void add_window      (Window *window);
    
    int     get_list_size ()        {return windows.size;};
    Window *get_list_elem (int idx) {return (Window *)list_get(&windows, idx);};

    void draw    (plug::TransformStack &transform_stack, plug::RenderTarget &target)     override;
    void onTick             (const plug::TickEvent &event, plug::EHC &ehc) override;
    void onMouseMove        (const plug::MouseMoveEvent &event, plug::EHC &ehc) override;
    void onMousePressed     (const plug::MousePressedEvent &event, plug::EHC &ehc) override;
    void onMouseReleased    (const plug::MouseReleasedEvent &event, plug::EHC &ehc) override;
    void onKeyboardPressed  (const plug::KeyboardPressedEvent &event, plug::EHC &ehc) override;
    void onKeyboardReleased (const plug::KeyboardReleasedEvent &event, plug::EHC &ehc) override;

    friend void init_canvases (Master_window *m_window, Tool_palette *palette);
};

#endif /* MASTER_WINDOW_H */