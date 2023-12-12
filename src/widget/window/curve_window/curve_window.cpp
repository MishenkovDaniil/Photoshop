#include "curve_window.h"

CurveWindow::CurveWindow (int width, int height, plug::Vec2d lh_pos, const char *w_name, 
                          Tool_palette *palette, plug::Canvas *edited_canvas, Curve_filter *curve_filter, CurveTool *curve_tool) :
    Window (width, height, lh_pos, w_name, false, palette),
    apply_button (plug::Vec2d (width / 2 - 30, 30), 60, 20, "apply", 
                  plug::Purple, plug::Purple, apply_filter_button, edited_canvas, new std::pair (curve_filter, curve_tool), PRESS_BUTTON)
{
    assert (palette); 
}

void CurveWindow::draw (plug::TransformStack &transform_stack, plug::RenderTarget &target) 
{
    Window::draw (transform_stack, target);
    
    transform_stack.enter (plug::Transform (layout_->getPosition ()));
    apply_button.draw (transform_stack, target);
    transform_stack.leave ();
}

void CurveWindow::onTick             (const plug::TickEvent &event, plug::EHC &ehc)
{
    ehc.stack.enter (plug::Transform (layout_->getPosition ()));
    apply_button.onTick (event, ehc);
    ehc.stack.leave ();
    Window::onTick (event, ehc);
}

void CurveWindow::onMouseMove        (const plug::MouseMoveEvent &event, plug::EHC &ehc)
{
    ehc.stack.enter (plug::Transform (layout_->getPosition ()));
    apply_button.onMouseMove (event, ehc);
    ehc.stack.leave ();
    Window::onMouseMove (event, ehc);
}

void CurveWindow::onMousePressed     (const plug::MousePressedEvent &event, plug::EHC &ehc)
{
    ehc.stack.enter (plug::Transform (layout_->getPosition ()));
    apply_button.onMousePressed (event, ehc);
    ehc.stack.leave ();
    Window::onMousePressed (event, ehc);
}

void CurveWindow::onMouseReleased    (const plug::MouseReleasedEvent &event, plug::EHC &ehc)
{
    ehc.stack.enter (plug::Transform (layout_->getPosition ()));
    apply_button.onMouseReleased (event, ehc);
    ehc.stack.leave ();
    Window::onMouseReleased (event, ehc);
}

void CurveWindow::onKeyboardPressed  (const plug::KeyboardPressedEvent &event, plug::EHC &ehc)
{
    ehc.stack.enter (plug::Transform (layout_->getPosition ()));
    apply_button.onKeyboardPressed (event, ehc);
    ehc.stack.leave ();
    Window::onKeyboardPressed (event, ehc);
}

void CurveWindow::onKeyboardReleased (const plug::KeyboardReleasedEvent &event, plug::EHC &ehc)
{
    ehc.stack.enter (plug::Transform (layout_->getPosition ()));
    apply_button.onKeyboardReleased (event, ehc);
    ehc.stack.leave ();
    Window::onKeyboardReleased (event, ehc);
}

bool apply_filter_button (void *widget, void *arg)
{
    assert (widget && arg);
    plug::Canvas *canvas = (plug::Canvas *)widget;
    std::pair<Curve_filter *, CurveTool *> *args = (std::pair<Curve_filter *, CurveTool *> *)arg;

    Curve_filter *curve_filter = args->first;
    CurveTool *curve_tool = args->second;

    CurvePlot &plot = curve_tool->getPlot ();

    curve_filter->applyCurveFilter (*canvas, plot);

    return true;    
}