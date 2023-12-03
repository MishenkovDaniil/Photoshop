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
    plug::Vec2d apply (const plug::Vec2d &vector) const override; 
    plug::Vec2d restore (const plug::Vec2d &vector) const override;
private:
    int get_size () const;
};

class Widget :public plug::Widget
{
protected:
    plug::LayoutBox *layout_ = nullptr;
public:
    virtual ~Widget () = default;
    virtual void draw (plug::TransformStack &transform_stack, plug::RenderTarget &target) = 0;
    
    virtual void onEvent (const plug::Event &event, plug::EHC &ehc);

    plug::LayoutBox &getLayoutBox ()                        override 
        {return *layout_;};
    const plug::LayoutBox& getLayoutBox() const             override
        {return *layout_;};
    void setLayoutBox (const plug::LayoutBox &new_layout)   override 
        {layout_->setPosition   (new_layout.getPosition ());
         layout_->setSize       (new_layout.getSize ());};

    virtual bool covers (plug::TransformStack& stack, const plug::Vec2d& position) const override;
    virtual void onParentUpdate (const plug::LayoutBox& parent_box) override;

protected:
    virtual void onTick             (const plug::TickEvent &event, plug::EHC &ehc) = 0;
    virtual void onMouseMove        (const plug::MouseMoveEvent &event, plug::EHC &ehc) = 0;
    virtual void onMousePressed     (const plug::MousePressedEvent &event, plug::EHC &ehc) = 0;
    virtual void onMouseReleased    (const plug::MouseReleasedEvent &event, plug::EHC &ehc) = 0;
    virtual void onKeyboardPressed  (const plug::KeyboardPressedEvent &event, plug::EHC &ehc) = 0;
    virtual void onKeyboardReleased (const plug::KeyboardReleasedEvent &event, plug::EHC &ehc) = 0;    
};

#endif /* WIDGET_H */