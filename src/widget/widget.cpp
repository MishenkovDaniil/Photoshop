#include "widget.h"

Transform Transform::combine(const Transform &parent_transform) const 
{
    Vec2d new_offset = Vec2d (offset_.get_x () * parent_transform.scale_.get_x (), offset_.get_y () * parent_transform.scale_.get_y ()) + parent_transform.offset_;
    return Transform (new_offset, Vec2d (scale_.get_x () * parent_transform.scale_.get_x (), scale_.get_y ()  * parent_transform.scale_.get_y ()));
}

Vec2d Transform::apply (const Vec2d &vector)
{
    return Vec2d ((vector.get_x () - offset_.get_x ()) * scale_.get_x (), 
                  (vector.get_y () - offset_.get_y ()) * scale_.get_y ());
}

Vec2d Transform::restore (const Vec2d &vector)
{
    return Vec2d (vector.get_x () + offset_.get_x (),
                  vector.get_y () + offset_.get_y ());
}


void TransformStack::enter (const Transform &transform)
{
    Transform top (Vec2d (0, 0));
    if (transform_stack.get_size () > 0)
    {
        top = transform_stack.top ();
    }

    Transform unite = transform.combine (top);
    transform_stack.push (unite);
}

void TransformStack::leave  ()
{
    if (transform_stack.get_size ()) 
        transform_stack.pop ();
}

Transform TransformStack::top () const
{
    return transform_stack.get_size () ? transform_stack.top () : Transform (Vec2d (0, 0));
}

int TransformStack::get_size () const
{
    return transform_stack.get_size ();
}

Vec2d TransformStack::apply (const Vec2d &vector) const
{
    return top ().apply (vector);
} 

Vec2d TransformStack::restore (const Vec2d &vector) const
{
    return top ().restore (vector);
}

void Widget::onEvent (const Event &event, EHC &ehc)
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