#ifndef LAYOUT_H
#define LAYOUT_H

#include "../../graphic_structures/vector/vector.h"
#include "../../graphic_structures/color/color.h"
// #include "../../vector.h"

class Layout_box
{
public:
    virtual void on_parent_update   (const Layout_box &parent_layout) = 0;
    virtual Vector get_position     () const = 0;
    virtual Vector get_size         () const = 0;
    virtual Layout_box *clone       () const = 0;
    virtual bool set_position       (const Vector &new_pos) = 0;
    virtual bool set_size           (const Vector &new_size) = 0;
};

class Default_layout_box : public Layout_box
{
    Vector relative_pos_;
    Vector relative_size_;
    Vector absolute_pos_;
    Vector absolute_size_;

public:
    Default_layout_box ();
    Default_layout_box (Vector relative_pos, Vector relative_size);
    ~Default_layout_box () = default;

    void on_parent_update   (const Layout_box &parent_layout) override;
    Vector get_position     () const override;
    Vector get_size         () const override;
    Layout_box *clone       () const override;
    bool set_position       (const Vector &new_pos);
    bool set_size           (const Vector &new_size);
};

#endif /* LAYOUT_H */