#include "widget.h"

plug::Transform plug::Transform::combine(const plug::Transform &parent_transform) const 
{
    plug::Vec2d new_offset = plug::Vec2d (offset.get_x () * parent_transform.scale.get_x (), offset.get_y () * parent_transform.scale.get_y ()) + parent_transform.offset;
    return plug::Transform (new_offset, plug::Vec2d (scale.get_x () * parent_transform.scale.get_x (), scale.get_y ()  * parent_transform.scale.get_y ()));
}

plug::Vec2d plug::Transform::apply (const plug::Vec2d &vector)
{
    return plug::Vec2d ((vector.get_x () - offset.get_x ()) * scale.get_x (), 
                        (vector.get_y () - offset.get_y ()) * scale.get_y ());
}

plug::Vec2d plug::Transform::restore (const plug::Vec2d &vector)
{
    return plug::Vec2d (vector.get_x () + offset.get_x (),
                        vector.get_y () + offset.get_y ());
}


void plug::TransformStack::enter (const plug::Transform &transform)
{
    plug::Transform top (plug::Vec2d (0, 0));
    if (transform_stack.get_size () > 0)
    {
        top = transform_stack.top ();
    }

    plug::Transform unite = transform.combine (top);
    transform_stack.push (unite);
}

void plug::TransformStack::leave  ()
{
    if (transform_stack.get_size ()) 
        transform_stack.pop ();
}

plug::Transform plug::TransformStack::top () const
{
    return transform_stack.get_size () ? transform_stack.top () : Transform (Vec2d (0, 0));
}

int plug::TransformStack::get_size () const
{
    return transform_stack.get_size ();
}

plug::Vec2d plug::TransformStack::apply (const plug::Vec2d &vector) const
{
    return top ().apply (vector);
} 

plug::Vec2d plug::TransformStack::restore (const plug::Vec2d &vector) const
{
    return top ().restore (vector);
}

void plug::Widget::onEvent (const plug::Event &event, plug::EHC &ehc)
{
    switch (event.getType ())
    {
        case Tick:
        {
            onTick ((const TickEvent &)event, ehc);
            break;
        }
        case MouseMove:
        {
            onMouseMove ((const MouseMoveEvent &)event, ehc);
            break;
        }
        case MousePressed:
        {
            onMousePressed ((const MousePressedEvent &)event, ehc);
            break;
        }
        case MouseReleased:
        {
            onMouseReleased ((const MouseReleasedEvent &)event, ehc);
            break;
        }
        case KeyboardPressed:
        {
            onKeyboardPressed ((const KeyboardPressedEvent &)event, ehc);
            break;
        }
        case KeyboardReleased:
        {
            onKeyboardReleased ((const KeyboardReleasedEvent &)event, ehc);
            break;
        }
        default:
            break;
    }
}