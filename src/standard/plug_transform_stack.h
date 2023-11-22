#ifndef PLUG_TRANSFORM_STACK_H
#define PLUG_TRANSFORM_STACK_H

#include "plug_transform.h"
#include "../graphic_structures/vector/vector.h"
#include "../vector.h"

static const double TRANSFORM_HEIGHT = 100;
static const double TRANSFORM_WIDTH  = 100;


namespace plug
{
    class TransformStack 
    {
        M_vector<Transform> transform_stack = M_vector<Transform> (Transform (-1, -1));
    public:
        virtual ~TransformStack () = default;
        virtual void enter (const Transform &transform) = 0;
        virtual void leave  () = 0;
        virtual Transform top () const = 0;
        virtual int get_size () const = 0;
        virtual Vec2d apply (const Vec2d &vector) const = 0; 
        virtual Vec2d restore (const Vec2d &vector) const = 0;
    };
}

#endif /* PLUG_TRANSFORM_STACK_H */