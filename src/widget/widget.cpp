#include "widget.h"

Transform Transform::unite(const Transform &prev) const 
{
    Vector new_offset = Vector (offset_.get_x () * prev.scale_.get_x (), offset_.get_y () * prev.scale_.get_y ()) + prev.offset_;
    return Transform (new_offset, Vector (scale_.get_x () * prev.scale_.get_x (), scale_.get_y ()  * prev.scale_.get_y ()));
}

Vector Transform::apply_transform (Vector &vector)
{
    return Vector (vector.get_x () - offset_.get_x (), vector.get_y () - offset_.get_y ());
}

Vector Transform::cancel_transform (Vector &vector)
{
    return Vector (vector.get_x () + offset_.get_x (), vector.get_y () + offset_.get_y ());
}

Vector Transform::scale_apply (Vector &vector) const
{
    return Vector (vector.get_x () * scale_.get_x (), vector.get_y () * scale_.get_y ());
}


void Transform_stack::push (Transform transform)
{
    Transform top (Vector (0, 0));
    if (transform_stack.get_size () > 0)
    {
        top = transform_stack.top ();
    }

    Transform unite = transform.unite (top);
    transform_stack.push (unite);
}

void Transform_stack::pop  ()
{
    if (transform_stack.get_size ()) 
        transform_stack.pop ();
}

Transform Transform_stack::top  ()
{
    return transform_stack.get_size () ? transform_stack.top () : Transform (Vector (0, 0));
}

int Transform_stack::get_size ()
{
    return transform_stack.get_size ();
}
