#include "../button.h"

static const char *font_path = "resources/font.ttf";
static const size_t CLOCK_CHARACTER_SIZE = 20;

class Clock : public Button
{
    float hours_ = 0;
    float minutes_ = 0;
    float seconds_ = 0;

    char hrs[3] = "";
    char min[3] = "";
    char sec[3] = "";

    char clck[20] = "";

public:
    Clock (Vector lh_corner, int width, int height, Button_run_fn func, Window *controlled_window, float hours, float minutes, float seconds, void *arg = nullptr, Color fill_color = Color (0, 0, 0, 0), int run_mask = RELEASE_BUTTON);
    ~Clock ();

    void change_time (float seconds, float minutes = -1, float hours = -1);

    void render (sf::RenderTarget &target, Transform_stack &transform_stack)                    override;
    bool on_mouse_pressed  (Mouse_key mouse_key, Vector &pos, Transform_stack &transform_stack) override;
    bool on_mouse_released (Mouse_key mouse_key, Vector &pos, Transform_stack &transform_stack) override;
    bool on_mouse_moved    (Vector &new_pos, Transform_stack &transform_stack)                  override;
    bool on_keyboard_pressed  (Keyboard_key key)              override;
    bool on_keyboard_released (Keyboard_key key)              override;
    bool on_time (float delta_sec)                            override;
};