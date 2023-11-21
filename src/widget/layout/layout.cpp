#include "layout.h"

plug::Default_layout_box::Default_layout_box () :
    relative_pos_   (plug::Vec2d (0, 0)),
    relative_size_  (plug::Vec2d (0, 0)),
    absolute_pos_   (plug::Vec2d (0, 0)),
    absolute_size_  (plug::Vec2d (0, 0)) {}

plug::Default_layout_box::Default_layout_box (plug::Vec2d relative_pos, plug::Vec2d relative_size) :
    relative_pos_   (relative_pos),
    relative_size_  (relative_size),
    absolute_pos_   (plug::Vec2d (0, 0)),
    absolute_size_  (plug::Vec2d (0, 0)) {}

void plug::Default_layout_box::on_parent_update   (const plug::Layout_box &parent_layout) 
{
    ;
}

plug::Vec2d plug::Default_layout_box::get_position     () const 
{
    return relative_pos_;
}

plug::Vec2d plug::Default_layout_box::get_size         () const 
{
    return relative_size_;
}

plug::Layout_box *plug::Default_layout_box::clone       () const 
{
    return nullptr;
}

bool plug::Default_layout_box::set_position       (const plug::Vec2d &new_pos) 
{
    relative_pos_ = new_pos;
    return true;
}

bool plug::Default_layout_box::set_size           (const plug::Vec2d &new_size) 
{
    relative_size_ = new_size;
    return true;
}
