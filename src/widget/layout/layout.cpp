#include "layout.h"

Default_layout_box::Default_layout_box () :
    relative_pos_   (Vec2d (0, 0)),
    relative_size_  (Vec2d (0, 0)),
    absolute_pos_   (Vec2d (0, 0)),
    absolute_size_  (Vec2d (0, 0)) {}

Default_layout_box::Default_layout_box (Vec2d relative_pos, Vec2d relative_size) :
    relative_pos_   (relative_pos),
    relative_size_  (relative_size),
    absolute_pos_   (Vec2d (0, 0)),
    absolute_size_  (Vec2d (0, 0)) {}

void Default_layout_box::on_parent_update   (const Layout_box &parent_layout) 
{
    ;
}

Vec2d Default_layout_box::get_position     () const 
{
    return relative_pos_;
}

Vec2d Default_layout_box::get_size         () const 
{
    return relative_size_;
}

Layout_box *Default_layout_box::clone       () const 
{
    return nullptr;
}

bool Default_layout_box::set_position       (const Vec2d &new_pos) 
{
    relative_pos_ = new_pos;
    return true;
}

bool Default_layout_box::set_size           (const Vec2d &new_size) 
{
    relative_size_ = new_size;
    return true;
}
