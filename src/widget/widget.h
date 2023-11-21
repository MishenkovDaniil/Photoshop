#ifndef WIDGET_H
#define WIDGET_H

#include <SFML/Graphics.hpp>
#include "constants.h"
#include "event.h"
#include "../graphic_structures/vector/vector.h"
#include "../graphic_structures/color/color.h"
#include "../vector.h"
#include "layout/layout.h"

static const double TRANSFORM_HEIGHT = 100;
static const double TRANSFORM_WIDTH  = 100;

class Transform 
{
    Vec2d offset_;
    Vec2d scale_;

public:
    Transform () {};
    Transform (const Vec2d offset, const Vec2d scale = Vec2d (1, 1)) : offset_ (offset), scale_ (scale) {};//
    // Transform (Vec2d lh_pos, Vec2d size)  : offset_ (lh_pos), scale_ (Vec2d (1, 1)) {}; //scale_ (TRANSFORM_WIDTH / size.get_x (), TRANSFORM_HEIGHT / size.get_y ()) {};
    Transform (const Transform &transform) : offset_ (transform.offset_), scale_ (transform.scale_) {};

    Vec2d getOffset () const {return offset_;};
    Vec2d getScale () const {return scale_;};
    void setOffset (const Vec2d &new_offset) {offset_ = new_offset;};
    void setScale (const Vec2d &new_scale) {scale_ = new_scale;};


    Transform combine(const Transform &parent_transform) const;
    Vec2d apply (const Vec2d &vector);
    Vec2d restore (const Vec2d &vector);
};

class TransformStack 
{
    M_vector<Transform> transform_stack = M_vector<Transform> (Transform (-1, -1));
public:
    void enter (const Transform &transform);
    void leave  ();
    Transform top () const;
    int get_size () const;
    Vec2d apply (const Vec2d &vector) const; 
    Vec2d restore (const Vec2d &vector) const;
};

class Widget 
{
protected:
    Layout_box *layout_ = nullptr;
public:
    virtual ~Widget () = default;
    virtual void render (sf::RenderTarget &target, TransformStack &transform_stack) = 0;
    
    virtual void onEvent (const Event &event, EHC &ehc);

    virtual void onTick             (const TickEvent &event, EHC &ehc) = 0;
    virtual void onMouseMove        (const MouseMoveEvent &event, EHC &ehc) = 0;
    virtual void onMousePressed     (const MousePressedEvent &event, EHC &ehc) = 0;
    virtual void onMouseReleased    (const MouseReleasedEvent &event, EHC &ehc) = 0;
    virtual void onKeyboardPressed  (const KeyboardPressedEvent &event, EHC &ehc) = 0;
    virtual void onKeyboardReleased (const KeyboardReleasedEvent &event, EHC &ehc) = 0;
    
    Layout_box &get_layout_box () {return *layout_;};
    void set_layout_box (Layout_box &new_layout) {layout_ = &new_layout;};
};

#endif /* WIDGET_H */