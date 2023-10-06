#ifndef MASTER_WINDOW_H
#define MASTER_WINDOW_H

#include "../../../list.h"
#include "../menu/menu.h"
#include "../canvas/canvas.h"
#include "../window.h"

static int INIT_WINDOWS_CAPACITY = 10;

class Master_window : public Window
{
    List windows = {};
    Menu *menu_ = nullptr;

public:
    Master_window (int width, int height, Vector lh_pos, const char *w_name, int list_capacity = INIT_WINDOWS_CAPACITY);
    ~Master_window () {};

    void add_menu_button (Button *button);

    void render (sf::RenderTarget &target) const override;
    bool on_mouse_pressed  (Mouse_key mouse_key, Vector &pos) override;
    bool on_mouse_released (Mouse_key mouse_key, Vector &pos) override;
    bool on_mouse_moved    (Vector &new_pos) override;    /// x, y - absolute values 
    bool on_keyboard_pressed  (Keyboard_key key) override;
    bool on_keyboard_released (Keyboard_key key) override;
    bool on_time (float delta_sec) override;
};

#endif /* MASTER_WINDOW_H */