#ifndef PLUG_CLOCK_H
#define PLUG_CLOCK_H

#include "../button.h"

static const char *FONT_PATH = "resources/font.ttf";
static const size_t CLOCK_CHARACTER_SIZE = 20;

class Clock : public Button
{
    float hours_ = 0;
    float minutes_ = 0;
    float seconds_ = 0;

    char hrs[3] = "";
    char min[3] = "";
    char sec[3] = "";

    char time_string[20] = "";

public:
    Clock (plug::Vec2d lh_corner, int width, int height, Button_run_fn func, Window *controlled_window, 
           float hours, float minutes, float seconds, void *arg = nullptr, 
           plug::Color fill_color = plug::Color (0, 0, 0, 0), int run_mask = RELEASE_BUTTON);
    ~Clock () = default;

    void change_time (float seconds, float minutes = -1, float hours = -1);

    void render (plug::RenderTarget &target, plug::TransformStack &transform_stack)                    override;
    void onTick             (const plug::TickEvent &event, plug::EHC &ehc) override;
    void onMouseMove        (const plug::MouseMoveEvent &event, plug::EHC &ehc) override;
    void onMousePressed     (const plug::MousePressedEvent &event, plug::EHC &ehc) override;
    void onMouseReleased    (const plug::MouseReleasedEvent &event, plug::EHC &ehc) override;
    void onKeyboardPressed  (const plug::KeyboardPressedEvent &event, plug::EHC &ehc) override;
    void onKeyboardReleased (const plug::KeyboardReleasedEvent &event, plug::EHC &ehc) override;
};

#endif /* PLUG_CLOCK_H */