#ifndef PLUG_WIDGET_H
#define PLUG_WIDGET_H

#include <SFML/Graphics.hpp>
#include "plug_event.h"
#include "plug_transform_stack.h"
#include "plug_layout.h"
#include "plug_rendertarget.h"

namespace plug
{
    class Widget 
    {
    public:
        virtual ~Widget () = default;
        virtual void render (sf::RenderTarget &target, TransformStack &transform_stack) = 0;
        
        virtual void onEvent (const Event &event, EHC &ehc) = 0;

        // virtual LayoutBox&       getLayoutBox()                     = 0;
        // virtual const LayoutBox& getLayoutBox() const               = 0;
        // virtual void             setLayoutBox(const LayoutBox& box) = 0;
        // virtual bool covers(TransformStack& stack, const Vec2d& position) const = 0;
        // virtual void onParentUpdate(const LayoutBox& parent_box) = 0;

        virtual LayoutBox &getLayoutBox () = 0;
        virtual void setLayoutBox (LayoutBox &new_layout) = 0;

    protected:
        virtual void onTick             (const TickEvent &event, EHC &ehc) = 0;
        virtual void onMouseMove        (const MouseMoveEvent &event, EHC &ehc) = 0;
        virtual void onMousePressed     (const MousePressedEvent &event, EHC &ehc) = 0;
        virtual void onMouseReleased    (const MouseReleasedEvent &event, EHC &ehc) = 0;
        virtual void onKeyboardPressed  (const KeyboardPressedEvent &event, EHC &ehc) = 0;
        virtual void onKeyboardReleased (const KeyboardReleasedEvent &event, EHC &ehc) = 0;    
    };
}

#endif /* PLUG_WIDGET_H */