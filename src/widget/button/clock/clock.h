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
    Clock (Vec2d lh_corner, int width, int height, Button_run_fn func, Window *controlled_window, float hours, float minutes, float seconds, void *arg = nullptr, Color fill_color = Color (0, 0, 0, 0), int run_mask = RELEASE_BUTTON);
    ~Clock () = default;

    void change_time (float seconds, float minutes = -1, float hours = -1);

    void render (sf::RenderTarget &target, TransformStack &transform_stack)                    override;
    void onTick             (const TickEvent &event, EHC &ehc) override;
    void onMouseMove        (const MouseMoveEvent &event, EHC &ehc) override;
    void onMousePressed     (const MousePressedEvent &event, EHC &ehc) override;
    void onMouseReleased    (const MouseReleasedEvent &event, EHC &ehc) override;
    void onKeyboardPressed  (const KeyboardPressedEvent &event, EHC &ehc) override;
    void onKeyboardReleased (const KeyboardReleasedEvent &event, EHC &ehc) override;
};