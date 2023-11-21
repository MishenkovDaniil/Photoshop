#ifndef CANVAS_H
#define CANVAS_H

#include "../../widget.h"
#include "../../button/button.h"
#include "../../../plugin/tools/tools.h"
#include "../../../plugin/tools/palette/palette.h"

namespace plug
{
    class Tool;
    class Tool_palette;
    class Master_window;
    class Window;
    
    struct SelectionMask
    {
        bool *mask = nullptr;
        size_t width_  = 0;
        size_t height_ = 0;

    public:
        SelectionMask (size_t width, size_t height) : width_ (width), height_ (height) 
                                                    { mask = new bool[width * height];
                                                    assert (mask);};
        ~SelectionMask () {delete[] mask;};
        size_t get_width () {return width_;};
        size_t get_height () {return height_;};

        bool get_pixel (size_t x, size_t y) const;  
        void set_pixel (size_t x, size_t y, bool flag);  
        void fill (bool value);
        //TODO :: void invert ();
    };

    class Canvas : public Widget
    {
        int width_ = 0;
        int height_ = 0;

        Color color_;

        Tool_palette *palette_;
        sf::IntRect draw_rect_;
        bool is_focused = false;
        SelectionMask selection;

    public:
        sf::RenderTexture canvas_texture;

    public: 
        Canvas (int width, int height, const Color color, const Vec2d lh_pos, Tool_palette *palette = nullptr);
        ~Canvas ();

        void render                 (sf::RenderTarget &target, TransformStack &transform_stack)    override;
        void onTick             (const TickEvent &event, EHC &ehc) override;
        void onMouseMove        (const MouseMoveEvent &event, EHC &ehc) override;
        void onMousePressed     (const MousePressedEvent &event, EHC &ehc) override;
        void onMouseReleased    (const MouseReleasedEvent &event, EHC &ehc) override;
        void onKeyboardPressed  (const KeyboardPressedEvent &event, EHC &ehc) override;
        void onKeyboardReleased (const KeyboardReleasedEvent &event, EHC &ehc) override;


        bool contains (int x, int y);
        
        Vec2d get_size () {return Vec2d (width_, height_);};
        sf::IntRect &get_draw_rect () {return draw_rect_;};
        void set_draw_rect_offset (int left, int top);
        void set_draw_rect_size   (int width, int height);
        void setSelectionMask (SelectionMask &new_selection) {selection = new_selection;};
        const SelectionMask &getSelectionMask () {return selection;};

        friend bool change_canvas_rect_up_down  (void *window, void *arg);
        friend bool change_canvas_rect_mid      (void *window, void *arg);
        friend bool change_canvas_rect_space    (void *window, void *arg);
        friend void init_canvases (Master_window *m_window, Tool_palette *palette);

        friend Window;
        friend Tool;
    };
}

#endif /* CANVAS_H */