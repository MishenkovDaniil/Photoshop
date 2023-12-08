#ifndef LAYOUT_H
#define LAYOUT_H

#include "../../graphic_structures/vector/vector.h"
#include "../../graphic_structures/color/color.h"
// #include "../../vector.h"
#include "../../standard/plug_layout.h"

class Default_layout_box : public plug::LayoutBox
{
    plug::Vec2d relative_pos_;
    plug::Vec2d relative_size_;
    plug::Vec2d absolute_pos_;
    plug::Vec2d absolute_size_;

public:
    Default_layout_box ();
    Default_layout_box (plug::Vec2d relative_pos, plug::Vec2d relative_size);
    Default_layout_box (const Default_layout_box &other);
    ~Default_layout_box () = default;

    void onParentUpdate   (const plug::LayoutBox &parent_layout) override;
    plug::Vec2d getPosition     () const override;
    plug::Vec2d getSize         () const override;
    plug::LayoutBox *clone       () const override;
    bool setPosition       (const plug::Vec2d &new_pos);
    bool setSize           (const plug::Vec2d &new_size);
};

#endif /* LAYOUT_H */