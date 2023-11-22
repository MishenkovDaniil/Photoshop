#include "layout.h"

Default_layout_box::Default_layout_box () :
    relative_pos_   (plug::Vec2d (0, 0)),
    relative_size_  (plug::Vec2d (0, 0)),
    absolute_pos_   (plug::Vec2d (0, 0)),
    absolute_size_  (plug::Vec2d (0, 0)) {}

Default_layout_box::Default_layout_box (plug::Vec2d relative_pos, plug::Vec2d relative_size) :
    relative_pos_   (relative_pos),
    relative_size_  (relative_size),
    absolute_pos_   (plug::Vec2d (0, 0)),
    absolute_size_  (plug::Vec2d (0, 0)) {}

void Default_layout_box::onParentUpdate   (const plug::LayoutBox &parent_layout) 
{
    //TODO
    ;
}

plug::Vec2d Default_layout_box::getPosition     () const 
{
    return relative_pos_;
}

plug::Vec2d Default_layout_box::getSize         () const 
{
    return relative_size_;
}

plug::LayoutBox *Default_layout_box::clone       () const 
{
    return nullptr;
}

bool Default_layout_box::setPosition       (const plug::Vec2d &new_pos) 
{
    relative_pos_ = new_pos;
    return true;
}

bool Default_layout_box::setSize           (const plug::Vec2d &new_size) 
{
    relative_size_ = new_size;
    return true;
}
