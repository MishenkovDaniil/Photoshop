#ifndef CURVE_WINDOW_H
#define CURVE_WINDOW_H

#include "../window.h"
#include "../canvas/canvas.h"

enum class CurveCanvas
{
    Red, 
    Green, 
    Blue
};

bool apply_filter_button (void *widget, void *arg);

class CurveWindow : public Window
{
    String_button apply_button;

public:
    CurveWindow (int width, int height, plug::Vec2d lh_pos, const char *w_name, 
                              Tool_palette *palette, plug::Canvas *edited_canvas, Curve_filter *curve_filter, CurveTool *curve_tool);
    ~CurveWindow () = default;

    void draw (plug::TransformStack &transform_stack, plug::RenderTarget &target)  override;
    void onTick             (const plug::TickEvent &event, plug::EHC &ehc) override;
    void onMouseMove        (const plug::MouseMoveEvent &event, plug::EHC &ehc) override;
    void onMousePressed     (const plug::MousePressedEvent &event, plug::EHC &ehc) override;
    void onMouseReleased    (const plug::MouseReleasedEvent &event, plug::EHC &ehc) override;
    void onKeyboardPressed  (const plug::KeyboardPressedEvent &event, plug::EHC &ehc) override;
    void onKeyboardReleased (const plug::KeyboardReleasedEvent &event, plug::EHC &ehc) override;
};

#endif /* CURVE_WINDOW_H */