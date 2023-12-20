#include "curve_window.h"

CurveWindow::CurveWindow (int width, int height, plug::Vec2d lh_pos, const char *w_name, 
                          Tool_palette *palette, plug::Canvas *edited_canvas, Curve_filter *curve_filter, CurveTool *curve_tool) :
    Window (width, height, lh_pos, w_name, false, palette),
    apply_button  (plug::Vec2d (0, 200), 60, 30, "APPLY", 
                  plug::Color (200, 200, 200), plug::Color (200, 200, 200), apply_filter_button, edited_canvas, 
                  new std::pair<void *, void *> (curve_filter, curve_tool), PRESS_BUTTON),
    cancel_button (plug::Vec2d (0, 230), 60, 30, "CANCEL", 
                  plug::Color (200, 200, 200), plug::Color (200, 200, 200), cancel_filter_button, 
                  new std::pair<Window * ,plug::Canvas *> (this, edited_canvas), 
                  new std::pair<void *, void *> (curve_filter, curve_tool), PRESS_BUTTON),
    reset_button  (plug::Vec2d (0, 260), 60, 30, "RESET", 
                  plug::Color (200, 200, 200), plug::Color (200, 200, 200), reset_filter_button, edited_canvas, 
                  new std::pair<void *, void *> (curve_filter, curve_tool), PRESS_BUTTON),
    menu_ (plug::Vec2d (width - 60 - 2, HEADER_HEIGHT), 60, height - HEADER_HEIGHT)
{
    menu_.setFillColor (plug::Transparent);
    menu_.setOutlineColor (plug::Transparent);
    menu_.add_button (&apply_button);
    menu_.add_button (&cancel_button);
    menu_.add_button (&reset_button);

    assert (palette); 
}

CurveWindow::~CurveWindow ()
{
    if (apply_button.get_arg ())
    {
        delete apply_button.get_arg ();
        apply_button.set_arg (nullptr);
    }
    if (cancel_button.get_arg ())
    {
        delete cancel_button.get_arg ();
        cancel_button.set_arg (nullptr);
    }
    if (cancel_button.get_widget ())
    {
        delete cancel_button.get_widget ();
        cancel_button.set_widget (nullptr);
    }
    if (reset_button.get_arg ())
    {
        delete reset_button.get_arg ();
        reset_button.set_arg (nullptr);
    }
}


void CurveWindow::draw (plug::TransformStack &transform_stack, plug::RenderTarget &target) 
{
    Window::draw (transform_stack, target);
    
    transform_stack.enter (plug::Transform (layout_->getPosition ()));
    menu_.draw (transform_stack, target);
    transform_stack.leave ();
}

void CurveWindow::onTick             (const plug::TickEvent &event, plug::EHC &ehc)
{
    ehc.stack.enter (plug::Transform (layout_->getPosition ()));
    menu_.onTick (event, ehc);
    ehc.stack.leave ();
    Window::onTick (event, ehc);
}

void CurveWindow::onMouseMove        (const plug::MouseMoveEvent &event, plug::EHC &ehc)
{
    ehc.stack.enter (plug::Transform (layout_->getPosition ()));
    menu_.onMouseMove (event, ehc);
    ehc.stack.leave ();
    Window::onMouseMove (event, ehc);
}

void CurveWindow::onMousePressed     (const plug::MousePressedEvent &event, plug::EHC &ehc)
{
    ehc.stack.enter (plug::Transform (layout_->getPosition ()));
    menu_.onMousePressed (event, ehc);
    ehc.stack.leave ();
    if (ehc.stopped)
        return;

    Window::onMousePressed (event, ehc);
}

void CurveWindow::onMouseReleased    (const plug::MouseReleasedEvent &event, plug::EHC &ehc)
{
    ehc.stack.enter (plug::Transform (layout_->getPosition ()));
    menu_.onMouseReleased (event, ehc);
    ehc.stack.leave ();
    Window::onMouseReleased (event, ehc);
}

void CurveWindow::onKeyboardPressed  (const plug::KeyboardPressedEvent &event, plug::EHC &ehc)
{
    ehc.stack.enter (plug::Transform (layout_->getPosition ()));
    menu_.onKeyboardPressed (event, ehc);
    ehc.stack.leave ();
    Window::onKeyboardPressed (event, ehc);
}

void CurveWindow::onKeyboardReleased (const plug::KeyboardReleasedEvent &event, plug::EHC &ehc)
{
    ehc.stack.enter (plug::Transform (layout_->getPosition ()));
    menu_.onKeyboardReleased (event, ehc);
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

bool cancel_filter_button (void *widget, void *arg)
{
    assert (widget && arg);
    std::pair <Window *, plug::Canvas *> *widgets = (std::pair <Window *, plug::Canvas *> *)widget;
    Window *window = widgets->first;
    plug::Canvas *canvas = widgets->second;

    std::pair<Curve_filter *, CurveTool *> *args = (std::pair<Curve_filter *, CurveTool *> *)arg;

    Curve_filter *curve_filter = args->first;
    CurveTool *curve_tool = args->second;

    CurvePlot &plot = curve_tool->getPlot ();

    curve_filter->resetCurveFilter (*canvas, plot);

    return window->get_header ()->closeWindow ();
}

bool reset_filter_button (void *widget, void *arg)
{
    assert (widget && arg);
    plug::Canvas *canvas = (plug::Canvas *)widget;
    std::pair<Curve_filter *, CurveTool *> *args = (std::pair<Curve_filter *, CurveTool *> *)arg;

    Curve_filter *curve_filter = args->first;
    CurveTool *curve_tool = args->second;

    CurvePlot &plot = curve_tool->getPlot ();
    plot.reset ();
    curve_filter->resetCurveFilter (*canvas, plot);
    curve_tool->onConfirm ();

    return true;    
}