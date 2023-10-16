#ifndef WIDGET_H
#define WIDGET_H

#include <SFML/Graphics.hpp>
#include "../graphic_structures/vector/vector.h"
#include "../graphic_structures/color/color.h"
#include "../vector.h"

static const Color BLACK = Color (0, 0, 0, 255);
static const Color WHITE = Color (255, 255, 255, 255);
static const double TRANSFORM_HEIGHT = 100;
static const double TRANSFORM_WIDTH  = 100;

enum Mouse_key
{
    Unknown_mouse_key = -1,
    M_Left, 
    M_Right,
    Middle
};

enum Keyboard_key
{
    Unknown = -1,
    A,
    B,
    C,
    D, 
    E,
    F,
    G,
    H,
    I,
    J,
    K,
    L,
    M,
    N,
    O,
    P,
    Q,
    R,
    S,
    T,
    U,
    V,
    W,
    X,
    Y,
    Z,
    Num0,
    Num1,
    Num2,
    Num3,
    Num4,
    Num5,
    Num6,
    Num7,
    Num8,
    Num9,
    Escape,
    LControl,
    LShift,
    LAlt,
    LSystem,
    RControl,
    RShift,
    RAlt,
    RSystem,
    // Menu,
    LBracket,
    RBracket,
    Semicolon,
    Comma,
    Period,
    Apostrophe,
    Slash,
    Backslash,
    Grave,
    Equal,
    Hyphen,
    Space,
    Enter,
    Backspace,
    Tab,
    PageUp,
    PageDown,
    End,
    Home,
    Insert,
    Delete,
    Add,
    Subtract,
    Multiply,
    Divide,
    Left,
    Right,
    Up,
    Down,
    Numpad0,
    Numpad1,
    Numpad2,
    Numpad3,
    Numpad4,
    Numpad5,
    Numpad6,
    Numpad7,
    Numpad8,
    Numpad9,
    F1,
    F2,
    F3,
    F4,
    F5,
    F6,
    F7,
    F8,
    F9,
    F10,
    F11,
    F12,
    F13,
    F14,
    F15,
    Pause,
    KeyCount
};

class Transform 
{
public:
    Vector offset_;
    Vector scale_;

    Transform () {};
    Transform (const Vector offset, const Vector scale = Vector (1, 1)) : offset_ (offset), scale_ (scale) {};//
    // Transform (Vector lh_pos, Vector size)  : offset_ (lh_pos), scale_ (Vector (1, 1)) {}; //scale_ (TRANSFORM_WIDTH / size.get_x (), TRANSFORM_HEIGHT / size.get_y ()) {};
    Transform (const Transform &transform) : offset_ (transform.offset_), scale_ (transform.scale_) {};

    Transform unite(const Transform &prev) const 
    {
        Vector new_offset = Vector (offset_.get_x () * prev.scale_.get_x (), offset_.get_y () * prev.scale_.get_y ()) + prev.offset_;
        return Transform (new_offset, Vector (scale_.get_x () * prev.scale_.get_x (), scale_.get_y ()  * prev.scale_.get_y ()));
    };
    Vector apply_transform (Vector &vector) {return Vector (vector.get_x () - offset_.get_x (), vector.get_y () - offset_.get_y ());};
};


class Widget 
{
public:
    virtual ~Widget () = default;
    virtual void render (sf::RenderTarget &target, M_vector<Transform> &transform_stack) = 0;
    virtual bool on_mouse_pressed  (Mouse_key mouse_key, Vector &pos) = 0;
    virtual bool on_mouse_released (Mouse_key mouse_key, Vector &pos) = 0;
    virtual bool on_mouse_moved    (Vector &new_pos) = 0;    /// x, y - absolute values 
    virtual bool on_keyboard_pressed  (Keyboard_key key) = 0;
    virtual bool on_keyboard_released (Keyboard_key key) = 0;
    virtual bool on_time (float delta_sec) = 0;
};

#endif /* WIDGET_H */