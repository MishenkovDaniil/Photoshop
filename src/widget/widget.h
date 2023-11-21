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


// namespace plug
// {
    class Transform 
    {
        Vec2d offset;
        Vec2d scale;

    public:
        Transform () {};
        Transform (const Vec2d offset_, const Vec2d scale_ = Vec2d (1, 1)) : offset (offset_), scale (scale_) {};//
        // Transform (Vec2d lh_pos, Vec2d size)  : offset_ (lh_pos), scale_ (Vec2d (1, 1)) {}; //scale_ (TRANSFORM_WIDTH / size.get_x (), TRANSFORM_HEIGHT / size.get_y ()) {};
        Transform (const Transform &transform) : offset (transform.offset), scale (transform.scale) {};

        Vec2d getOffset () const {return offset;};
        Vec2d getScale () const {return scale;};
        void setOffset (const Vec2d &new_offset) {offset = new_offset;};
        void setScale (const Vec2d &new_scale) {scale = new_scale;};

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

        // virtual LayoutBox&       getLayoutBox()                     = 0;
        // virtual const LayoutBox& getLayoutBox() const               = 0;
        // virtual void             setLayoutBox(const LayoutBox& box) = 0;
        // virtual bool covers(TransformStack& stack, const Vec2d& position) const = 0;
        // virtual void onParentUpdate(const LayoutBox& parent_box) = 0;

        Layout_box &get_layout_box () {return *layout_;};
        void set_layout_box (Layout_box &new_layout) {layout_ = &new_layout;};

    protected:
        virtual void onTick             (const TickEvent &event, EHC &ehc) = 0;
        virtual void onMouseMove        (const MouseMoveEvent &event, EHC &ehc) = 0;
        virtual void onMousePressed     (const MousePressedEvent &event, EHC &ehc) = 0;
        virtual void onMouseReleased    (const MouseReleasedEvent &event, EHC &ehc) = 0;
        virtual void onKeyboardPressed  (const KeyboardPressedEvent &event, EHC &ehc) = 0;
        virtual void onKeyboardReleased (const KeyboardReleasedEvent &event, EHC &ehc) = 0;    
    };
// }

#endif /* WIDGET_H */