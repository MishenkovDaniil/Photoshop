#ifndef EVENT_H
#define EVENT_H

#include <iostream>
#include "../graphic_structures/vector/vector.h"
#include "constants.h"

class TransformStack;

class Event 
{
    const size_t m_type;
public:
    Event (size_t type) : m_type (type) {};                                            
    size_t getType () const {return m_type;};
};

struct EHC              /// event handling context
{
    EHC () = default;
    EHC (TransformStack &stack_, bool stopped_, bool overlapped_) : stack (stack_), stopped (stopped_), overlapped (overlapped_) {};
    EHC (TransformStack &stack_) : stack (stack_), stopped (false), overlapped (false) {};

    TransformStack &stack;
    bool stopped;   
    bool overlapped; 
};

enum EventType 
{
    Tick 		    = 0,
    MouseMove 	    = 1,
    MousePressed 	= 2,
    MouseReleased  	= 3,
    KeyboardPressed = 4,
    KeyboardReleased = 5
};

struct MouseMoveEvent : public Event 
{
    MouseMoveEvent () : Event (MouseMove) {};

    bool shift, ctrl, alt;
    Vec2d pos;
};

struct MousePressedEvent : public Event 
{
    MousePressedEvent () : Event (MousePressed) {};

    MouseButton button_id;
    bool shift, ctrl, alt;
    Vec2d pos;
};

struct MouseReleasedEvent : public Event 
{
    MouseReleasedEvent () : Event (MouseReleased) {};

    MouseButton button_id;
    bool shift, ctrl, alt;
    Vec2d pos;
};

struct KeyboardPressedEvent : public Event 
{
    KeyboardPressedEvent () : Event (KeyboardPressed) {};

    KeyCode key_id;
    bool shift, ctrl, alt;
};

struct KeyboardReleasedEvent : public Event 
{
    KeyboardReleasedEvent () : Event (KeyboardReleased) {};
    
    KeyCode key_id;
    bool shift, ctrl, alt;
};

struct TickEvent : public Event 
{
    TickEvent () : Event (Tick) {};

    double delta_time;
};

#endif /* EVENT_H */