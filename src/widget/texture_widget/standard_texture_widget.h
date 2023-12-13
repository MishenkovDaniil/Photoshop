#include <iostream>

#ifndef PLUGRENDERTEXTURE_WIDGET_H
#define PLUGRENDERTEXTURE_WIDGET_H

#include "../../standard/plug_widget.h"
#include "../../vector.h"

class PlugRenderTexture : public plug::Widget
{
    bool is_changed_ = false;
    M_vector <plug::VertexArray> arrays = M_vector <plug::VertexArray> (plug::VertexArray (plug::Points, 0));

public:
    PlugRenderTexture ()  {};
    ~PlugRenderTexture () = default;

    void onEvent(const plug::Event &event, plug::EHC &context)  {};
    void onParentUpdate(const plug::LayoutBox &parent_box)  {};

    plug::LayoutBox &getLayoutBox() {};
    const plug::LayoutBox &getLayoutBox() const  {};
    void setLayoutBox(const plug::LayoutBox &box){};

    bool covers(plug::TransformStack &stack, const plug::Vec2d &position) const {};
    void draw (plug::TransformStack &transform_stack, plug::RenderTarget &target); 
    void onTick             (const plug::TickEvent &event, plug::EHC &ehc) {return;};
    void onMouseMove        (const plug::MouseMoveEvent &event, plug::EHC &ehc) {return;};
    void onMousePressed     (const plug::MousePressedEvent &event, plug::EHC &ehc) {return;};
    void onMouseReleased    (const plug::MouseReleasedEvent &event, plug::EHC &ehc) {return;};
    void onKeyboardPressed  (const plug::KeyboardPressedEvent &event, plug::EHC &ehc) {return;};
    void onKeyboardReleased (const plug::KeyboardReleasedEvent &event, plug::EHC &ehc) {return;};
    
    void draw (plug::VertexArray& array) {arrays.push (array);};
    void clear () {while (arrays.get_size ()) arrays.pop ();};
};

#endif /* PLUGRENDERTEXTURE_WIDGET_H */