#ifndef BUTTON_H
#define BUTTON_H

enum Button_type 
{
    Unknown_button = - 1,
    Brush
};

#include <SFML/Graphics.hpp>

class Window;
class Menu;
class List_button;
class Tool_palette;
class Button_palette;

#include "../constants.h"
#include "../widget.h"
#include "../window/window.h"
#include "../../graphic_structures/color/color.h"
#include "../../graphic_structures/point/point.h"
#include "../../graphic_structures/vector/vector.h"

static const int START_CAPACITY = 10;
static const int BUTTON_TEXT_SIZE = 10;

typedef bool (*Button_run_fn) (void *widget, void *arg);

// TODO::
// think of rm button frame and frame is needed make rectangle and set button on it
static int MOVE_BUTTON = 0b010;
static int RELEASE_BUTTON = 0b100;
static int PRESS_BUTTON = 0b001;


class Button :public Widget
{
protected:
    int width_ = 0;
    int height_ = 0;

    bool is_pressed_ = false;
    Vec2d press_pos_;
    Color fill_color_;

    Button_run_fn run_fn_ = nullptr;
    void *controlled_widget_ = nullptr;
    void *arg_ = nullptr;
    
    int run_mask_ = 0;

public:
    Button () {}; //TODO: make button_create for this constructor case
    Button (Vec2d lh_corner, int width, int height, Button_run_fn func, void *controlled_widget,
            void *arg = nullptr, Color fill_color = Black, int run_mask = RELEASE_BUTTON);
    
    virtual ~Button () {delete layout_;};

    bool contains (double x, double y) const;
    Button_run_fn get_func          ()const {return run_fn_;};
    Color         get_fill_color    ()const {return fill_color_;};
    bool          get_status        ()const {return is_pressed_;}; //may be instead made bool is_pressed (...) const {...};
    int           get_width         ()const {assert (layout_); return layout_->get_size ().get_x ();};
    int           get_height        ()const {assert (layout_); return layout_->get_size ().get_y ();};

    void          set_arg           (void *arg) {arg_ = arg;};
    
    bool run ();
    void render (sf::RenderTarget &target, TransformStack &transform_stack)                    override;
    // bool on_mouse_pressed  (MouseButton mouse_button, Vec2d &pos, TransformStack &transform_stack) override;
    // bool on_mouse_released (MouseButton mouse_button, Vec2d &pos, TransformStack &transform_stack) override;
    // bool on_mouse_moved    (Vec2d &new_pos, TransformStack &transform_stack)                  override;
    // bool on_keyboard_pressed  (KeyCode key)              override;
    // bool on_keyboard_released (KeyCode key)              override;
    // bool on_tick (float delta_sec)                            override;

    void onTick             (TickEvent &event, EHC &ehc) override;
    void onMouseMove        (MouseMoveEvent &event, EHC &ehc) override;
    void onMousePressed     (MousePressedEvent &event, EHC &ehc) override;
    void onMouseReleased    (MouseReleasedEvent &event, EHC &ehc) override;
    void onKeyboardPressed  (KeyboardPressedEvent &event, EHC &ehc) override;
    void onKeyboardReleased (KeyboardReleasedEvent &event, EHC &ehc) override;

    friend Scrollbar;
    friend Menu;
    friend List_button;
    friend Button_palette;
};

class Texture_button : public Button
{
protected:
    sf::Texture *cur_texture_ = nullptr;
    sf::Texture &pressed_texture_;
    sf::Texture &released_texture_;
    sf::Sprite *sprite_ = nullptr;

public:
    Texture_button (Vec2d lh_corner, int width, int height, sf::Texture &pressed, sf::Texture &released, 
                    Button_run_fn func, void *controlled_widget, void *arg = nullptr, int run_mask = RELEASE_BUTTON);
    ~Texture_button () override;

    void render (sf::RenderTarget &target, TransformStack &transform_stack) override;
    // bool on_mouse_pressed  (MouseButton mouse_button, Vec2d &pos, TransformStack &transform_stack) override;
    // bool on_mouse_released (MouseButton mouse_button, Vec2d &pos, TransformStack &transform_stack) override;
    // bool on_mouse_moved    (Vec2d &new_pos, TransformStack &transform_stack)                  override;
    // bool on_keyboard_pressed  (KeyCode key)              override;
    // bool on_keyboard_released (KeyCode key)              override;
    void onMouseMove        (MouseMoveEvent &event, EHC &ehc) override;
    void onMousePressed     (MousePressedEvent &event, EHC &ehc) override;
    void onMouseReleased    (MouseReleasedEvent &event, EHC &ehc) override;
    void onKeyboardPressed  (KeyboardPressedEvent &event, EHC &ehc) override;
    void onKeyboardReleased (KeyboardReleasedEvent &event, EHC &ehc) override;
};

class String_button : public Button
{
protected:
    const Color *cur_color_ = nullptr;
    const Color &pressed_color_;
    const Color &released_color_;
    
    char *string_ = nullptr;
    int str_size_ = 0;
    
public:
    String_button (Vec2d lh_corner, int width, int height, const char *string, 
                   const Color &pressed_color, const Color &released_color, Button_run_fn func, 
                   void *controlled_widget, void *arg = nullptr, int run_mask = RELEASE_BUTTON);
    ~String_button ();

    void render (sf::RenderTarget &target, TransformStack &transform_stack) override;
    // bool on_mouse_pressed  (MouseButton mouse_button, Vec2d &pos, TransformStack &transform_stack) override;
    // bool on_mouse_released (MouseButton mouse_button, Vec2d &pos, TransformStack &transform_stack) override;
    // bool on_mouse_moved    (Vec2d &new_pos, TransformStack &transform_stack)                  override;
    // bool on_keyboard_pressed  (KeyCode key)              override;
    // bool on_keyboard_released (KeyCode key)              override;
    void onMouseMove        (MouseMoveEvent &event, EHC &ehc) override;
    void onMousePressed     (MousePressedEvent &event, EHC &ehc) override;
    void onMouseReleased    (MouseReleasedEvent &event, EHC &ehc) override;
    void onKeyboardPressed  (KeyboardPressedEvent &event, EHC &ehc) override;
    void onKeyboardReleased (KeyboardReleasedEvent &event, EHC &ehc) override;
};

class List_button : public Button
{
    Button *list_button_ = nullptr;
    M_vector<Button *> buttons_ = M_vector<Button *> (nullptr);
    bool is_open_ = false;
    size_t relative_height_ = 0;

public:
    List_button (Button *list_button);
    ~List_button ();

    void add_button (Button *button); /// can change button layout
    
    void render (sf::RenderTarget &target, TransformStack &transform_stack)                        override;
    // bool on_mouse_pressed     (MouseButton mouse_button, Vec2d &pos, TransformStack &transform_stack)  override;
    // bool on_mouse_released    (MouseButton mouse_button, Vec2d &pos, TransformStack &transform_stack)  override;
    // bool on_mouse_moved       (Vec2d &new_pos, TransformStack &transform_stack)                   override;    
    // bool on_keyboard_pressed  (KeyCode key)                                                    override;
    // bool on_keyboard_released (KeyCode key)                                                    override;
    // bool on_tick (float delta_sec)                                                                  override;
    void onTick             (TickEvent &event, EHC &ehc) override;
    void onMouseMove        (MouseMoveEvent &event, EHC &ehc) override;
    void onMousePressed     (MousePressedEvent &event, EHC &ehc) override;
    void onMouseReleased    (MouseReleasedEvent &event, EHC &ehc) override;
    void onKeyboardPressed  (KeyboardPressedEvent &event, EHC &ehc) override;
    void onKeyboardReleased (KeyboardReleasedEvent &event, EHC &ehc) override;
};

#endif /* BUTTON_H */