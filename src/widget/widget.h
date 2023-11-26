#ifndef WIDGET_H
#define WIDGET_H

#include <SFML/Graphics.hpp>

#include "../graphic_structures/vector/vector.h"
#include "../vector.h"
#include "layout/layout.h"

#include "../standard/plug_event.h"
#include "../standard/plug_widget.h"
#include "../standard/plug_transform_stack.h"
#include "../standard/plug_transform.h"

class TransformStack : public plug::TransformStack
{
    M_vector<plug::Transform> transform_stack = M_vector<plug::Transform> (plug::Transform (-1, -1));
public:
    ~TransformStack () = default;
    void enter (const plug::Transform &transform) override;
    void leave  () override;
    plug::Transform top () const override;
    int get_size () const override;
    plug::Vec2d apply (const plug::Vec2d &vector) const override; 
    plug::Vec2d restore (const plug::Vec2d &vector) const override;
};

class Widget :public plug::Widget
{
protected:
    plug::LayoutBox *layout_ = nullptr;
public:
    virtual ~Widget () = default;
    virtual void render (plug::RenderTarget &target, plug::TransformStack &transform_stack) = 0;
    
    virtual void onEvent (const plug::Event &event, plug::EHC &ehc);

    // virtual LayoutBox&       getLayoutBox()                     = 0;
    // virtual const LayoutBox& getLayoutBox() const               = 0;
    // virtual void             setLayoutBox(const LayoutBox& box) = 0;
    // virtual bool covers(TransformStack& stack, const Vec2d& position) const = 0;
    // virtual void onParentUpdate(const LayoutBox& parent_box) = 0;

    plug::LayoutBox &getLayoutBox () {return *layout_;};
    void setLayoutBox (plug::LayoutBox &new_layout) {layout_ = &new_layout;};

protected:
    virtual void onTick             (const plug::TickEvent &event, plug::EHC &ehc) = 0;
    virtual void onMouseMove        (const plug::MouseMoveEvent &event, plug::EHC &ehc) = 0;
    virtual void onMousePressed     (const plug::MousePressedEvent &event, plug::EHC &ehc) = 0;
    virtual void onMouseReleased    (const plug::MouseReleasedEvent &event, plug::EHC &ehc) = 0;
    virtual void onKeyboardPressed  (const plug::KeyboardPressedEvent &event, plug::EHC &ehc) = 0;
    virtual void onKeyboardReleased (const plug::KeyboardReleasedEvent &event, plug::EHC &ehc) = 0;    
};

#endif /* WIDGET_H */