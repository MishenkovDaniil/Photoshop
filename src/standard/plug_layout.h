#ifndef PLUG_LAYOUT_H
#define PLUG_LAYOUT_H

#include "../graphic_structures/vector/vector.h"

namespace plug
{
    class LayoutBox
    {
    public:
        virtual ~LayoutBox () = default;
        virtual void onParentUpdate   (const LayoutBox &parent_layout) = 0;
        virtual Vec2d getPosition      () const = 0;
        virtual Vec2d getSize          () const = 0;
        virtual LayoutBox *clone       () const = 0;
        virtual bool setPosition       (const Vec2d &new_pos) = 0;
        virtual bool setSize           (const Vec2d &new_size) = 0;
    };
}

#endif /* PLUG_LAYOUT_H */