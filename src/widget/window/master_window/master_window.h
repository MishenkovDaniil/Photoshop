#ifndef MASTER_WINDOW_H
#define MASTER_WINDOW_H

class Tool_palette;
#include "../../../list.h"
#include "../menu/menu.h"
#include "../canvas/canvas.h"
#include "../window.h"
#include "../../../plugin/tools/palette/palette.h"

static int INIT_WINDOWS_CAPACITY = 10;

class Master_window : public Window
{
    List windows = {};
    Menu *menu_ = nullptr;

    size_t first_ = 1;
    size_t last_  = 0;

public:
    Master_window (int width, int height, Vec2d lh_pos, const char *w_name, int list_capacity = INIT_WINDOWS_CAPACITY);
    ~Master_window ();

    void add_menu_button (Button *button);
    void add_window      (Window *window);
    
    int     get_list_size ()        {return windows.size;};
    Window *get_list_elem (int idx) {return (Window *)list_get(&windows, idx);};

    void render    (sf::RenderTarget &target, TransformStack &transform_stack)     override;
    void onTick             (const TickEvent &event, EHC &ehc) override;
    void onMouseMove        (const MouseMoveEvent &event, EHC &ehc) override;
    void onMousePressed     (const MousePressedEvent &event, EHC &ehc) override;
    void onMouseReleased    (const MouseReleasedEvent &event, EHC &ehc) override;
    void onKeyboardPressed  (const KeyboardPressedEvent &event, EHC &ehc) override;
    void onKeyboardReleased (const KeyboardReleasedEvent &event, EHC &ehc) override;

    friend void init_canvases (Master_window *m_window, Tool_palette *palette);
};

#endif /* MASTER_WINDOW_H */