
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

    void add_widget (Widget *widget);   /// stores widget * so if widget pointer is destroyed it will be destroyed in manager too 
    // void rm_widget (Widget *widget);
    void render (sf::RenderTarget &target);

    void onEvent (sf::Event *event, double delta_time);
    void onTick             (const TickEvent &event, EHC &ehc);
    void onMouseMove        (const MouseMoveEvent &event, EHC &ehc);
    void onMousePressed     (const MousePressedEvent &event, EHC &ehc);
    void onMouseReleased    (const MouseReleasedEvent &event, EHC &ehc);
    void onKeyboardPressed  (const KeyboardPressedEvent &event, EHC &ehc);
    void onKeyboardReleased (const KeyboardReleasedEvent &event, EHC &ehc);
};

#endif /* WIDGET_MANAGER_H */