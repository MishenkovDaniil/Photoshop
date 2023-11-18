#ifndef LAYOUT_H
#define LAYOUT_H

#include "../../graphic_structures/vector/vector.h"
#include "../../graphic_structures/color/color.h"
// #include "../../vector.h"

class Layout_box
{
public:
    virtual ~Layout_box () = default;
    virtual void on_parent_update   (const Layout_box &parent_layout) = 0;
    virtual Vec2d get_position     () const = 0;
    virtual Vec2d get_size         () const = 0;
    virtual Layout_box *clone       () const = 0;
    virtual bool set_position       (const Vec2d &new_pos) = 0;
    virtual bool set_size           (const Vec2d &new_size) = 0;
};

class Default_layout_box : public Layout_box
{
    Vec2d relative_pos_;
    Vec2d relative_size_;
    Vec2d absolute_pos_;
    Vec2d absolute_size_;

public:
    Default_layout_box ();
    Default_layout_box (Vec2d relative_pos, Vec2d relative_size);
    ~Default_layout_box () = default;

    void on_parent_update   (const Layout_box &parent_layout) override;
    Vec2d get_position     () const override;
    Vec2d get_size         () const override;
    Layout_box *clone       () const override;
    bool set_position       (const Vec2d &new_pos);
    bool set_size           (const Vec2d &new_size);
};

#endif /* LAYOUT_H */