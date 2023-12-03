
#ifndef WIDGET_MANAGER_H
#define WIDGET_MANAGER_H

#include "../widget.h"
#include "../../list.h"
#include "../../vector.h"
static const int INIT_WIDGETS_CAPACITY = 10;

class Widget_manager
{
    List widgets = {};
    TransformStack transform_stack_;

public:
    Widget_manager (int list_capacity = INIT_WIDGETS_CAPACITY);
    ~Widget_manager ();

    void add_widget (plug::Widget *widget);   /// stores widget * so if widget pointer is destroyed it will be destroyed in manager too 
    // void rm_widget (Widget *widget);
    void draw (plug::RenderTarget &target);

    void onEvent (sf::Event *event, double delta_time);
    void onTick             (const plug::TickEvent &event, plug::EHC &ehc);
    void onMouseMove        (const plug::MouseMoveEvent &event, plug::EHC &ehc);
    void onMousePressed     (const plug::MousePressedEvent &event, plug::EHC &ehc);
    void onMouseReleased    (const plug::MouseReleasedEvent &event, plug::EHC &ehc);
    void onKeyboardPressed  (const plug::KeyboardPressedEvent &event, plug::EHC &ehc);
    void onKeyboardReleased (const plug::KeyboardReleasedEvent &event, plug::EHC &ehc);
};

#endif /* WIDGET_MANAGER_H */