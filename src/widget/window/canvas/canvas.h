#ifndef CANVAS_H
#define CANVAS_H

#include "../../widget.h"
#include "../../button/button.h"
#include "../../../plugin/tools/tools.h"
#include "../../../plugin/tools/palette/palette.h"
// #include "../../../standard/plug_canvas.h"

#include "../../../graphics/rendertexture/rendertexture.h"
#include "../../../graphics/intrect/intrect.h"
#include "../../../graphics/sprite/sfml_texture.h"
#include "../../../standard/standard.h"

class Tool;
class Tool_palette;
class Master_window;
class Window;


struct SelectionMask : public plug::SelectionMask
{
    bool *mask = nullptr;
    size_t width_  = 0;
    size_t height_ = 0;

public:
    SelectionMask (size_t width, size_t height) : width_ (width), height_ (height) 
                                                { mask = new bool[width * height];
                                                assert (mask);};
    ~SelectionMask () {delete[] mask;};
    size_t getWidth () const override {return width_;};
    size_t getHeight () const override {return height_;};

    bool getPixel (size_t x, size_t y) const override;  
    void setPixel (size_t x, size_t y, bool flag) override;  
    void fill (bool value) override;
    virtual void invert(void) override {return;}; //TODO

    //TODO :: void invert ();
};

class CanvasView;
class Canvas : public plug::Canvas
{
    int width_ = 0;
    int height_ = 0;

    plug::Color color_;

    IntRect draw_rect_;
    SelectionMask selection;
    plug::Texture canvas_img;
    Sprite canvas_sprite;

public:
    bool is_changed_img = false;
    bool is_changed_texture = false;
    RenderTexture canvas_texture;

public:
    Canvas (int width, int height, const plug::Color color);
    ~Canvas () = default;

    plug::Vec2d getSize () const                                                    override 
        {return plug::Vec2d (width_, height_);};

    const plug::SelectionMask &getSelectionMask ()                                  override 
        {return selection;};
    void setSize(const plug::Vec2d &size)                                           override 
        {width_ = size.x; height_ = size.y;};
    unsigned int getNativeHandle(void) const                                        override;                
    plug::Color getPixel(size_t x, size_t y) const                                  override;                
    const plug::Texture& getTexture(void) const                                     override;
    void setPixel(size_t x, size_t y, const plug::Color& color)                     override;   
    void draw (const plug::VertexArray& vertex_array)                               override;             
    void draw (const plug::VertexArray& vertex_array, const plug::Texture &texture) override;
    void draw (Drawable &drawable);
    
    plug::Vec2d getFullSize () const {return canvas_texture.getSize ();};
    IntRect &getDrawRect () {return draw_rect_;};
    void setDrawRectOffset (int left, int top);
    void setDrawRectSize   (int width, int height);

private:
    void update_texture ();
    void update_img ();
    void dump_img ();
     
    friend CanvasView;
    friend Window;
    friend Tool;
};

class CanvasView : public Widget
{
    Tool_palette *palette_;
    bool is_focused = false;

    Canvas view;
public: 
    CanvasView (int width, int height, const plug::Color color, const plug::Vec2d lh_pos, Tool_palette *palette = nullptr);
    ~CanvasView ();

    void render             (plug::RenderTarget &target, plug::TransformStack &transform_stack) override;
    void onTick             (const plug::TickEvent &event, plug::EHC &ehc) override;
    void onMouseMove        (const plug::MouseMoveEvent &event, plug::EHC &ehc) override;
    void onMousePressed     (const plug::MousePressedEvent &event, plug::EHC &ehc) override;
    void onMouseReleased    (const plug::MouseReleasedEvent &event, plug::EHC &ehc) override;
    void onKeyboardPressed  (const plug::KeyboardPressedEvent &event, plug::EHC &ehc) override;
    void onKeyboardReleased (const plug::KeyboardReleasedEvent &event, plug::EHC &ehc) override;

    virtual void onEvent (const plug::Event &event, plug::EHC &ehc) {Widget::onEvent (event, ehc);};

    plug::LayoutBox &getLayoutBox () {return *layout_;};
    void setLayoutBox (plug::LayoutBox &new_layout) {layout_ = &new_layout;};

    plug::Vec2d getSize () const {return view.getSize ();};
    plug::Vec2d getFullSize () const {return view.getFullSize ();};
    void setSize (const plug::Vec2d &size) {layout_->setSize (size); view.setSize (size);}

    IntRect &getDrawRect () {return view.getDrawRect ();};
    void setDrawRectOffset (int left, int top);
    void setDrawRectSize   (int width, int height);
    const plug::SelectionMask &getSelectionMask () {return view.getSelectionMask ();};
    const plug::Texture& getTexture(void) const {return view.getTexture ();};

    friend bool change_canvas_rect_up_down  (void *window, void *arg);
    friend bool change_canvas_rect_mid      (void *window, void *arg);
    friend bool change_canvas_rect_space    (void *window, void *arg);
    friend void init_canvases (Master_window *m_window, Tool_palette *palette);

    // friend Window;
    friend Tool;
};

#endif /* CANVAS_H */