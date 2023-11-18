#ifndef MASTER_WINDOW_H
#define MASTER_WINDOW_H

class Tool_palette;
#include "../../../list.h"
#include "../menu/menu.h"
#include "../canvas/canvas.h"
#include "../window.h"
#include "../tools/palette/palette.h"

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
    // bool on_mouse_pressed     (MouseButton mouse_button, Vec2d &pos, TransformStack &transform_stack)                        override;
    // bool on_mouse_released    (MouseButton mouse_button, Vec2d &pos, TransformStack &transform_stack)                        override;
    // bool on_mouse_moved       (Vec2d &new_pos, TransformStack &transform_stack)                                         override;
    // bool on_keyboard_pressed  (KeyCode key)                                        override;
    // bool on_keyboard_released (KeyCode key)                                        override;
    // bool on_tick (float delta_sec)                                                      override;

    void onTick             (TickEvent &event, EHC &ehc) override;
    void onMouseMove        (MouseMoveEvent &event, EHC &ehc) override;
    void onMousePressed     (MousePressedEvent &event, EHC &ehc) override;
    void onMouseReleased    (MouseReleasedEvent &event, EHC &ehc) override;
    void onKeyboardPressed  (KeyboardPressedEvent &event, EHC &ehc) override;
    void onKeyboardReleased (KeyboardReleasedEvent &event, EHC &ehc) override;

    friend void init_canvases (Master_window *m_window, Tool_palette *palette);
};

#endif /* MASTER_WINDOW_H */