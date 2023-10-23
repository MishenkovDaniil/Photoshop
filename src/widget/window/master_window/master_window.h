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
    size_t last_ = 0;

public:
    Master_window (int width, int height, Vector lh_pos, const char *w_name, int list_capacity = INIT_WINDOWS_CAPACITY);
    ~Master_window ();

    void add_menu_button (Button *button);
    void add_window (Window *window);
    
    int get_list_size () {return windows.size;};
    Window *get_list_elem (int idx) {return (Window *)list_get(&windows, idx);};

    void render (sf::RenderTarget &target, M_vector<Transform> &transform_stack) override;
    bool on_mouse_pressed  (Mouse_key mouse_key, Vector &pos) override;
    bool on_mouse_released (Mouse_key mouse_key, Vector &pos) override;
    bool on_mouse_moved    (Vector &new_pos) override;    /// x, y - absolute values 
    bool on_keyboard_pressed  (Keyboard_key key) override;
    bool on_keyboard_released (Keyboard_key key) override;
    bool on_time (float delta_sec) override;

    friend bool brush_button_act (void *m_window, void *arg);
    friend void init_canvases (Master_window *m_window, Tool_palette *palette);
};

#endif /* MASTER_WINDOW_H */