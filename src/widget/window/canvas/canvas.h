#ifndef CANVAS_H
#define CANVAS_H

#include "../../widget.h"
#include "../../button/button.h"
#include "../../../plugin/tools/tools.h"
#include "../../../plugin/tools/palette/palette.h"
// #include "../../../standard/plug_canvas.h"

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

class CanvasView;
class Canvas : public plug::Canvas
{
    int width_ = 0;
    int height_ = 0;

    plug::Color color_;

    sf::IntRect draw_rect_;
    SelectionMask selection;

public:
    sf::RenderTexture canvas_texture;

public:
    Canvas (int width, int height, const plug::Color color);
    ~Canvas () = default;
    bool contains (int x, int y) override;

    
    plug::Vec2d get_size () override {return plug::Vec2d (width_, height_);};
    sf::IntRect &get_draw_rect () override {return draw_rect_;};
    void set_draw_rect_offset (int left, int top) override;
    void set_draw_rect_size   (int width, int height) override;
    void setSelectionMask (SelectionMask &new_selection) override {selection = new_selection;};
    const SelectionMask &getSelectionMask () override{return selection;};

    sf::RenderTexture &getRenderTexture () override {return canvas_texture;};
    friend CanvasView;
    friend Window;
    friend Tool;
};

class CanvasView : public Widget
{
    // int width_ = 0;
    // int height_ = 0;

    // plug::Color color_;

    Tool_palette *palette_;
    // sf::IntRect draw_rect_;
    bool is_focused = false;
    // SelectionMask selection;

// public:
    // sf::RenderTexture canvas_texture;
    Canvas view;
public: 
    CanvasView (int width, int height, const plug::Color color, const plug::Vec2d lh_pos, Tool_palette *palette = nullptr);
    ~CanvasView ();

    void render             (sf::RenderTarget &target, plug::TransformStack &transform_stack) override;
    void onTick             (const plug::TickEvent &event, plug::EHC &ehc) override;
    void onMouseMove        (const plug::MouseMoveEvent &event, plug::EHC &ehc) override;
    void onMousePressed     (const plug::MousePressedEvent &event, plug::EHC &ehc) override;
    void onMouseReleased    (const plug::MouseReleasedEvent &event, plug::EHC &ehc) override;
    void onKeyboardPressed  (const plug::KeyboardPressedEvent &event, plug::EHC &ehc) override;
    void onKeyboardReleased (const plug::KeyboardReleasedEvent &event, plug::EHC &ehc) override;

    virtual void onEvent (const plug::Event &event, plug::EHC &ehc) {Widget::onEvent (event, ehc);};

    plug::LayoutBox &getLayoutBox () {return *layout_;};
    void setLayoutBox (plug::LayoutBox &new_layout) {layout_ = &new_layout;};

    bool contains (int x, int y);
    
    sf::RenderTexture &getRenderTexture () {return view.getRenderTexture ();};
    plug::Vec2d get_size () {return view.get_size ();};
    sf::IntRect &get_draw_rect () {return view.get_draw_rect ();};
    void set_draw_rect_offset (int left, int top);
    void set_draw_rect_size   (int width, int height);
    void setSelectionMask (SelectionMask &new_selection) {view.setSelectionMask (new_selection);};
    const SelectionMask &getSelectionMask () {return view.getSelectionMask ();};

    friend bool change_canvas_rect_up_down  (void *window, void *arg);
    friend bool change_canvas_rect_mid      (void *window, void *arg);
    friend bool change_canvas_rect_space    (void *window, void *arg);
    friend void init_canvases (Master_window *m_window, Tool_palette *palette);

    friend Window;
    friend Tool;
};

#endif /* CANVAS_H */