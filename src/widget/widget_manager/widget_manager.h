
#ifndef WIDGET_MANAGER_H
#define WIDGET_MANAGER_H

#include "../widget.h"
#include "../../list.h"
#include "../../vector.h"
static const int INIT_WIDGETS_CAPACITY = 10;

class Widget_manager : public Widget
{
    List widgets = {};
    M_vector<Transform> transform_stack_;

public:
    Widget_manager (int list_capacity = INIT_WIDGETS_CAPACITY);
    ~Widget_manager ();

    void add_widget (Widget *widget);   /// stores widget * so if widget pointer is destroyed it will be destroyed in manager too 
    // void rm_widget (Widget *widget);
    void render (sf::RenderTarget &target, M_vector<Transform> &transform_stack);
    bool on_mouse_pressed  (Mouse_key mouse_key, Vector &pos, M_vector<Transform> &transform_stack);
    bool on_mouse_released (Mouse_key mouse_key, Vector &pos, M_vector<Transform> &transform_stack);
    bool on_mouse_moved    (Vector &new_pos, M_vector<Transform> &transform_stack);    /// x, y - absolute values 
    bool on_keyboard_pressed  (Keyboard_key key) override;
    bool on_keyboard_released (Keyboard_key key) override;
    bool on_time (float delta_sec);
};

#endif /* WIDGET_MANAGER_H */