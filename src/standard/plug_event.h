#ifndef PLUG_EVENT_H
#define PLUG_EVENT_H

#include <iostream>

#include "../graphic_structures/vector/vector.h"
#include "plug_mouse_button.h"
#include "plug_key_code.h"


namespace plug {

class TransformStack;
/**
 * @brief Context of event handling
 */
struct EHC {
  TransformStack &stack; /*!< Current state of transform stack */

  bool stopped; /*!< Event is intercepted and should not be handled further */

  bool overlapped; /*!< Event position is contained within area managed by one
                      of the handlers */
};


/**
 * @brief Constants for types of predefined events
 */
enum EventType {
  Tick = 0,             /*!< TickEvent */
  MouseMove = 1,        /*!< MouseMoveEvent */
  MousePressed = 2,     /*!< MousePressedEvent */
  MouseReleased = 3,    /*!< MouseReleasedEvent */
  KeyboardPressed = 4,  /*!< KeyboardPressedEvent */
  KeyboardReleased = 5, /*!< KeyboardReleasedEvent */
};

/**
 * @brief Base class for all events
 */
class Event {
public:
  Event(size_t type) : m_type(type) {}

  /**
   * @brief Retrieve type of underlying event
   *
   * @return Retrieved type
   */
  size_t getType(void) const { return m_type; }

private:
  const size_t m_type;
};

/**
 * @brief Event of mouse changing position
 */
struct MouseMoveEvent : public Event {
  MouseMoveEvent(const Vec2d &position, bool shift_pressed, bool ctrl_pressed,
                 bool alt_pressed)
      : Event(MouseMove), shift(shift_pressed), ctrl(ctrl_pressed),
        alt(alt_pressed), pos(position) {}

  bool shift; /*!< Shift is pressed */
  bool ctrl;  /*!< Ctrl is pressed */
  bool alt;   /*!< Alt is pressed */
  Vec2d pos;  /*!< New mouse position */
};

/**
 * @brief Event of mouse button being pressed
 */
struct MousePressedEvent : public Event {
  MousePressedEvent(MouseButton button, const Vec2d &position,
                    bool shift_pressed, bool ctrl_pressed, bool alt_pressed)
      : Event(MousePressed), button_id(button), shift(shift_pressed),
        ctrl(ctrl_pressed), alt(alt_pressed), pos(position) {}
  MouseButton button_id; /*!< Pressed button */
  bool shift;            /*!< Shift is pressed */
  bool ctrl;             /*!< Ctrl is pressed */
  bool alt;              /*!< Alt is pressed */
  Vec2d pos;             /*!< Mouse position when pressed */
};

/**
 * @brief Event of mouse button being released
 */
struct MouseReleasedEvent : public Event {
  MouseReleasedEvent(MouseButton button, const Vec2d &position,
                     bool shift_pressed, bool ctrl_pressed, bool alt_pressed)
      : Event(MouseReleased), button_id(button), shift(shift_pressed),
        ctrl(ctrl_pressed), alt(alt_pressed), pos(position) {}
  MouseButton button_id; /*!< Released button */
  bool shift;            /*!< Shift is pressed */
  bool ctrl;             /*!< Ctrl is pressed */
  bool alt;              /*!< Alt is pressed */
  Vec2d pos;             /*!< Mouse position when released */
};

/**
 * @brief Event of keyboard button being pressed
 */
struct KeyboardPressedEvent : public Event {
  KeyboardPressedEvent(KeyCode key, bool shift_pressed, bool ctrl_pressed,
                       bool alt_pressed)
      : Event(KeyboardPressed), key_id(key), shift(shift_pressed),
        ctrl(ctrl_pressed), alt(alt_pressed) {}
  KeyCode key_id; /*!< Pressed key */
  bool shift;     /*!< Shift is pressed */
  bool ctrl;      /*!< Ctrl is pressed */
  bool alt;       /*!< Alt is pressed */
};

/**
 * @brief Event of keyboard button being released
 */
struct KeyboardReleasedEvent : public Event {
  KeyboardReleasedEvent(KeyCode key, bool shift_pressed, bool ctrl_pressed,
                        bool alt_pressed)
      : Event(KeyboardReleased), key_id(key), shift(shift_pressed),
        ctrl(ctrl_pressed), alt(alt_pressed) {}
  KeyCode key_id; /*!< Released key */
  bool shift;     /*!< Shift is pressed */
  bool ctrl;      /*!< Ctrl is pressed */
  bool alt;       /*!< Alt is pressed */
};

/**
 * @brief Event of time passing
 */
struct TickEvent : public Event {
  TickEvent(double seconds) : Event(Tick), delta_time(seconds) {}

  double delta_time; /*!< Time (in seconds) since last TickEvent */
};

} // namespace plug

// namespace plug
// {
//     class TransformStack;
//     class Event 
//     {
//         const size_t m_type;
//     public:
//         Event (size_t type) : m_type (type) {};                                            
//         size_t getType () const {return m_type;};
//     };

//     struct EHC              /// event handling context
//     {
//         EHC () = default;
//         EHC (TransformStack &stack_, bool stopped_, bool overlapped_) : stack (stack_), stopped (stopped_), overlapped (overlapped_) {};
//         EHC (TransformStack &stack_) : stack (stack_), stopped (false), overlapped (false) {};

//         TransformStack &stack;
//         bool stopped;   
//         bool overlapped; 
//     };

//     enum EventType 
//     {
//         Tick 		    = 0,
//         MouseMove 	    = 1,
//         MousePressed 	= 2,
//         MouseReleased  	= 3,
//         KeyboardPressed = 4,
//         KeyboardReleased = 5
//     };

//     struct MouseMoveEvent : public Event 
//     {
//         MouseMoveEvent () : Event (MouseMove) {};

//         bool shift, ctrl, alt;
//         Vec2d pos;
//     };

//     struct MousePressedEvent : public Event 
//     {
//         MousePressedEvent () : Event (MousePressed) {};

//         MouseButton button_id;
//         bool shift, ctrl, alt;
//         Vec2d pos;
//     };

//     struct MouseReleasedEvent : public Event 
//     {
//         MouseReleasedEvent () : Event (MouseReleased) {};

//         MouseButton button_id;
//         bool shift, ctrl, alt;
//         Vec2d pos;
//     };

//     struct KeyboardPressedEvent : public Event 
//     {
//         KeyboardPressedEvent () : Event (KeyboardPressed) {};

//         KeyCode key_id;
//         bool shift, ctrl, alt;
//     };

//     struct KeyboardReleasedEvent : public Event 
//     {
//         KeyboardReleasedEvent () : Event (KeyboardReleased) {};
        
//         KeyCode key_id;
//         bool shift, ctrl, alt;
//     };

//     struct TickEvent : public Event 
//     {
//         TickEvent () : Event (Tick) {};

//         double delta_time;
//     };
// }

#endif /* PLUG_EVENT_H */