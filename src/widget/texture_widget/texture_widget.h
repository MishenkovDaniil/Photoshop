#ifndef TEXTURE_WIDGET_H
#define TEXTURE_WIDGET_H

#include "../widget.h"

namespace plug
{
    class M_render_texture : public Widget
    {
    public:
        sf::RenderTexture texture_;
        M_render_texture (int width, int height, Color color)  {texture_.create (width, height); texture_.clear (color);};
        ~M_render_texture () = default;

        void clear (Color color) {texture_.clear (color);};
        void render (sf::RenderTarget &target, TransformStack &transform_stack); 
        void onTick             (const TickEvent &event, EHC &ehc) {return;};
        void onMouseMove        (const MouseMoveEvent &event, EHC &ehc) {return;};
        void onMousePressed     (const MousePressedEvent &event, EHC &ehc) {return;};
        void onMouseReleased    (const MouseReleasedEvent &event, EHC &ehc) {return;};
        void onKeyboardPressed  (const KeyboardPressedEvent &event, EHC &ehc) {return;};
        void onKeyboardReleased (const KeyboardReleasedEvent &event, EHC &ehc) {return;};
    };
}

#endif /* TEXTURE_WIDGET_H */