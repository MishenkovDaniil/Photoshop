#include "layout.h"


Default_layout_box::Default_layout_box () :
    relative_pos_   (Vector (0, 0)),
    relative_size_  (Vector (0, 0)),
    absolute_pos_   (Vector (0, 0)),
    absolute_size_  (Vector (0, 0)) {}
Default_layout_box::Default_layout_box (Vector relative_pos, Vector relative_size) :
    relative_pos_   (relative_pos),
    relative_size_  (relative_size),
    absolute_pos_   (Vector (0, 0)),
    absolute_size_  (Vector (0, 0)) {}

void Default_layout_box::on_parent_update   (const Layout_box &parent_layout) 
{
    ;
}

Vector Default_layout_box::get_position     () const 
{
    return relative_pos_;
}

Vector Default_layout_box::get_size         () const 
{
    return relative_size_;
}

Layout_box *Default_layout_box::clone       () const 
{
    return nullptr;
}

bool Default_layout_box::set_position       (const Vector &new_pos) 
{
    relative_pos_ = new_pos;
    return true;
}

bool Default_layout_box::set_size           (const Vector &new_size) 
{
    relative_size_ = new_size;
    return true;
}
