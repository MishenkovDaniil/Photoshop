#ifndef WIDGET_H
#define WIDGET_H

#include <SFML/Graphics.hpp>
#include "../graphic_structures/vector/vector.h"
#include "../graphic_structures/color/color.h"

static const Color BLACK = Color (0, 0, 0, 255);
static const Color WHITE = Color (255, 255, 255, 255);

enum Mouse_key
{
    Unknown_mouse_key = -1,
    Left, 
    Right,
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
    Menu,
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

class Widget 
{
public:
virtual void render (sf::RenderTarget &target) const = 0;
virtual bool on_mouse_pressed  (Mouse_key mouse_key, Vector &pos) = 0;
virtual bool on_mouse_released (Mouse_key mouse_key, Vector &pos) = 0;
virtual bool on_mouse_moved    (Vector &new_pos) = 0;    /// x, y - absolute values 
virtual bool on_keyboard_pressed  (Keyboard_key key) = 0;
virtual bool on_keyboard_released (Keyboard_key key) = 0;
virtual bool on_time (float delta_sec) = 0;
};

#endif /* WIDGET_H */