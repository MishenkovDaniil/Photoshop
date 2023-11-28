#include "widget.h"

void TransformStack::enter (const plug::Transform &transform)
{
    plug::Transform top (plug::Vec2d (0, 0));
    if (transform_stack.get_size () > 0)
    {
        top = transform_stack.top ();
    }

    plug::Transform unite = transform.combine (top);
    transform_stack.push (unite);
}

void TransformStack::leave  ()
{
    if (transform_stack.get_size ()) 
        transform_stack.pop ();
}

plug::Transform TransformStack::top () const
{
    return transform_stack.get_size () ? transform_stack.top () : plug::Transform (plug::Vec2d (0, 0));
}

int TransformStack::get_size () const
{
    return transform_stack.get_size ();
}

plug::Vec2d TransformStack::apply (const plug::Vec2d &vector) const
{
    return top ().apply (vector);
} 

plug::Vec2d TransformStack::restore (const plug::Vec2d &vector) const
{
    return top ().restore (vector);
}

void Widget::onEvent (const plug::Event &event, plug::EHC &ehc)
{
    switch (event.getType ())
    {
        case plug::Tick:
        {
            onTick ((const plug::TickEvent &)event, ehc);
            break;
        }
        case plug::MouseMove:
        {
            onMouseMove ((const plug::MouseMoveEvent &)event, ehc);
            break;
        }
        case plug::MousePressed:
        {
            onMousePressed ((const plug::MousePressedEvent &)event, ehc);
            break;
        }
        case plug::MouseReleased:
        {
            onMouseReleased ((const plug::MouseReleasedEvent &)event, ehc);
            break;
        }
        case plug::KeyboardPressed:
        {
            onKeyboardPressed ((const plug::KeyboardPressedEvent &)event, ehc);
            break;
        }
        case plug::KeyboardReleased:
        {
            onKeyboardReleased ((const plug::KeyboardReleasedEvent &)event, ehc);
            break;
        }
        default:
            break;
    }
}

bool Widget::covers (plug::TransformStack& stack, const plug::Vec2d& position) const
{
    plug::Vec2d top_left_corner    = stack.restore (layout_->getPosition ());
    plug::Vec2d top_right_corner   = stack.restore (layout_->getPosition () + layout_->getSize () * plug::Vec2d (1, 0));
    plug::Vec2d low_left_corner    = stack.restore (layout_->getPosition () + layout_->getSize () * plug::Vec2d (0, 1));

    return ((position.x >= top_left_corner.x)  && 
            (position.x <= top_right_corner.x) && 
            (position.y <= low_left_corner.y)  && 
            (position.y >= top_left_corner.y));
}

void Widget::onParentUpdate(const plug::LayoutBox& parent_box)
{
    //TODO
    ;
}