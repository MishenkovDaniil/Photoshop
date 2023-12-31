#ifndef BUTTON_H
#define BUTTON_H


// #include <SFML/Graphics.hpp>
#include <cstring>

#include "../constants.h"
#include "../widget.h"
#include "../window/window.h"

#include "../../graphic_structures/color/color.h"
#include "../../graphic_structures/vector/vector.h"

#include "../../standard/plug_key_code.h"
#include "../../standard/plug_mouse_button.h"

#include "../../graphics/sprite/sfml_texture.h"

static const int START_CAPACITY = 10;
static const int BUTTON_TEXT_SIZE = 12;

typedef bool (*Button_run_fn) (void *widget, void *arg);

// TODO::
// think of rm button frame and frame is needed make rectangle and set button on it
static int MOVE_BUTTON = 0b010;
static int RELEASE_BUTTON = 0b100;
static int PRESS_BUTTON = 0b001;

class Window;
class Menu;
class List_button;
class Tool_palette;
class Button_palette;
class Scrollbar;

enum Button_type 
{
    Unknown_button = - 1,
    Brush
};

class Button :public Widget
{
protected:
    int width_ = 0;
    int height_ = 0;

    bool is_pressed_ = false;
    plug::Vec2d press_pos_;
    plug::Color fill_color_ = plug::Color (0, 0, 0);

    Button_run_fn run_fn_ = nullptr;
    void *controlled_widget_ = nullptr;
    void *arg_ = nullptr;
    
    int run_mask_ = 0;

public:
    Button () {}; //TODO: make button_create for this constructor case
    Button (plug::Vec2d lh_corner, int width, int height, Button_run_fn func, void *controlled_widget,
            void *arg = nullptr, plug::Color fill_color = plug::Black, int run_mask = RELEASE_BUTTON);
    
    virtual ~Button () {delete layout_;};

    Button_run_fn get_func          ()const {return run_fn_;};
    plug::Color   get_fill_color    ()const {return fill_color_;};
    bool          get_status        ()const {return is_pressed_;}; //may be instead made bool is_pressed (...) const {...};
    int           get_width         ()const {assert (layout_); return layout_->getSize ().get_x ();};
    int           get_height        ()const {assert (layout_); return layout_->getSize ().get_y ();};

    void          set_arg           (void *arg) {arg_ = arg;};
    void          *get_arg          ()  {return arg_;};
    void          *get_widget          ()  {return controlled_widget_;};
    void          *set_widget          (void *widget)  {controlled_widget_ = widget;};
    
    bool run ();
    void draw (plug::TransformStack &transform_stack, plug::RenderTarget &target) override;
    void onTick             (const plug::TickEvent &event, plug::EHC &ehc) override;
    void onMouseMove        (const plug::MouseMoveEvent &event, plug::EHC &ehc) override;
    void onMousePressed     (const plug::MousePressedEvent &event, plug::EHC &ehc) override;
    void onMouseReleased    (const plug::MouseReleasedEvent &event, plug::EHC &ehc) override;
    void onKeyboardPressed  (const plug::KeyboardPressedEvent &event, plug::EHC &ehc) override;
    void onKeyboardReleased (const plug::KeyboardReleasedEvent &event, plug::EHC &ehc) override;

    friend Scrollbar;
    friend Menu;
    friend List_button;
    friend Button_palette;
};

class Texture_button : public Button
{
protected:
    Sprite *pressed_sprite_ = nullptr;
    Sprite *released_sprite_ = nullptr;
    Sprite *cur_sprite_ = nullptr;

public:
    Texture_button (plug::Vec2d lh_corner, int width, int height, plug::Texture &pressed, plug::Texture &released, 
                    Button_run_fn func, void *controlled_widget, void *arg = nullptr, int run_mask = RELEASE_BUTTON);
    Texture_button (plug::Vec2d lh_corner, int width, int height, Sprite &pressed, Sprite &released, 
                    Button_run_fn func, void *controlled_widget, void *arg = nullptr, int run_mask = RELEASE_BUTTON);
    ~Texture_button () override;

    void draw (plug::TransformStack &transform_stack, plug::RenderTarget &target) override;
    void onMouseMove        (const plug::MouseMoveEvent &event, plug::EHC &ehc) override;
    void onMousePressed     (const plug::MousePressedEvent &event, plug::EHC &ehc) override;
    void onMouseReleased    (const plug::MouseReleasedEvent &event, plug::EHC &ehc) override;
    void onKeyboardPressed  (const plug::KeyboardPressedEvent &event, plug::EHC &ehc) override;
    void onKeyboardReleased (const plug::KeyboardReleasedEvent &event, plug::EHC &ehc) override;
};

class String_button : public Button
{
protected:
    const plug::Color *cur_color_ = nullptr;
    plug::Color pressed_color_;
    plug::Color released_color_;
    
    char *string_ = nullptr;
    int str_size_ = 0;
    
public:
    String_button (plug::Vec2d lh_corner, int width, int height, const char *string, 
                const plug::Color &pressed_color, const plug::Color &released_color, Button_run_fn func, 
                void *controlled_widget, void *arg = nullptr, int run_mask = RELEASE_BUTTON);
    String_button (String_button &other) = default;
    ~String_button ();

    void draw (plug::TransformStack &transform_stack, plug::RenderTarget &target) override;
    void onMouseMove        (const plug::MouseMoveEvent &event, plug::EHC &ehc) override;
    void onMousePressed     (const plug::MousePressedEvent &event, plug::EHC &ehc) override;
    void onMouseReleased    (const plug::MouseReleasedEvent &event, plug::EHC &ehc) override;
    void onKeyboardPressed  (const plug::KeyboardPressedEvent &event, plug::EHC &ehc) override;
    void onKeyboardReleased (const plug::KeyboardReleasedEvent &event, plug::EHC &ehc) override;

    String_button &operator = (const String_button &other) 
        {
            pressed_color_ = other.pressed_color_;
            pressed_color_ = other.pressed_color_;
            if (string_)
                delete[] string_;
            string_ = new char[strlen (other.string_) + 1];
            strcpy (string_, other.string_);
            str_size_ = other.str_size_;

            if (layout_)
                delete layout_;

            layout_ = other.layout_->clone ();

            return *this;
        };
};

class List_button : public Button
{
    Button *list_button_ = nullptr;
    bool is_open_ = false;
    size_t relative_height_ = 0;

public:
    M_vector<Button *> buttons_ = M_vector<Button *> (nullptr);
    List_button (Button *list_button);
    ~List_button ();

    void add_button (Button *button); /// can change button layout
    
    void draw (plug::TransformStack &transform_stack, plug::RenderTarget &target)                        override;
    void onTick             (const plug::TickEvent &event, plug::EHC &ehc) override;
    void onMouseMove        (const plug::MouseMoveEvent &event, plug::EHC &ehc) override;
    void onMousePressed     (const plug::MousePressedEvent &event, plug::EHC &ehc) override;
    void onMouseReleased    (const plug::MouseReleasedEvent &event, plug::EHC &ehc) override;
    void onKeyboardPressed  (const plug::KeyboardPressedEvent &event, plug::EHC &ehc) override;
    void onKeyboardReleased (const plug::KeyboardReleasedEvent &event, plug::EHC &ehc) override;

    int getSize () const {return buttons_.get_size ();};
};

#endif /* BUTTON_H */