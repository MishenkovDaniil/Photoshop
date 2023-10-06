#ifndef BUTTON_H
#define BUTTON_H

enum Button_type 
{
    Unknown_button = - 1,
    Brush
};

class Piston;
class Mol_manager;

#include <SFML/Graphics.hpp>

#include "../widget.h"
#include "../../graphic_structures/color/color.h"
#include "../../graphic_structures/point/point.h"
#include "../../graphic_structures/vector/vector.h"

static const int START_CAPACITY = 10;
static const Color Black = Color (0, 0, 0, 255);

typedef bool (*Button_run_fn) (Point &object, sf::Keyboard::Key key);

class Button :public Widget
{
protected:
    Vector lh_corner_;    /// z coord is not used
    // Point rl_corner_ = Point (0, 0);    /// 
    int width_ = 0;
    int height_ = 0;

    bool is_pressed_ = false;
    Color fill_color_;
    Button_run_fn run_fn_ = nullptr;

public:
    Button () {}; //TODO: make button_create for this constructor case
    Button (Vector lh_corner, int width, int height, Button_run_fn func, Color fill_color = Black) :
            lh_corner_ (lh_corner),
            width_ (width),
            height_ (height),
            run_fn_ (func),
            fill_color_ (fill_color) {};
    virtual ~Button () {};

    bool get_status () const {return is_pressed_;}; //may be instead made bool is_pressed (...) const {...};
    bool contains (double x, double y) const;

    // virtual bool run (Point &object, sf::Keyboard::Key key) = 0;
    // virtual void update (bool is_pressed) = 0;
    bool run (Point &object, sf::Keyboard::Key key);
    void render (sf::RenderTarget &target)              const override;
    bool on_mouse_pressed  (Mouse_key mouse_key, Vector &pos) override;
    bool on_mouse_released (Mouse_key mouse_key, Vector &pos) override;
    bool on_mouse_moved    (Vector &new_pos)                  override;
    bool on_keyboard_pressed  (Keyboard_key key)              override;
    bool on_keyboard_released (Keyboard_key key)              override;
    bool on_time (float delta_sec)                            override;
};

// class Texture_button : public Button
// {
// protected:
//     sf::Texture *cur_texture_ = nullptr;
//     sf::Texture &pressed_texture_;
//     sf::Texture &released_texture_;
//     sf::Sprite *sprite = nullptr;

// public:
//     Texture_button (Point lh_corner, Point rl_corner, sf::Texture &pressed, sf::Texture &released) : 
//                     Button (lh_corner, rl_corner),
//                     pressed_texture_ (pressed),
//                     released_texture_ (released) 
//                     {cur_texture_ = &released_texture_;
//                     sprite = new sf::Sprite;};
//     ~Texture_button () override {delete sprite;};

//     void draw (sf::RenderWindow &window, int screen_w, int screen_h)const override;
//     void update (bool is_pressed) override final;
//     bool run (Point &object, sf::Keyboard::Key key) override = 0;
// };

// class String_button : public Button
// {
// protected:
//     Color *cur_color_ = nullptr;
//     Color &pressed_color_;
//     Color &released_color_;
    
//     char *string_ = nullptr;
//     int str_size = 0;
    
// public:
//     String_button (Point lh_corner, Point rl_corner, Color &pressed_color, Color &released_color, const char *string = nullptr) :
//                    Button (lh_corner, rl_corner),
//                    pressed_color_ (pressed_color),
//                    released_color_ (released_color)
//                    { cur_color_ = &released_color_;
//                     if (!string) return; 
//                     str_size = std::strlen(string);
//                     string_ = new char[str_size + 1];
//                     assert (string_);
//                     std::strcpy (string_, string); 
//                    };
//     ~String_button () {if (string_) delete[] string_;};

//     void draw (sf::RenderWindow &window, int screen_w, int screen_h)const override;
//     void update (bool is_pressed) override final;
//     bool run (Point &object, sf::Keyboard::Key key) override = 0;
// };

// class Button_manager 
// {
//     Button **buttons_p = nullptr;
//     int size_ = 0;
//     int capacity_ = 0;

//     sf::RenderWindow *window_ = nullptr;
//     int screen_h_ = 0;
//     int screen_w_ = 0;

// public: 
//     Button_manager (sf::RenderWindow &window, int screen_w, int screen_h);
//     ~Button_manager ();

//     void add (Button *button);
//     void draw ();
//     bool run (Point object = Point (0, 0, 0), sf::Keyboard::Key key = sf::Keyboard::Key::Unknown);
//     Button *contains (double x, double y);
// // private:
//     // bool resize ();
// };

#endif /* BUTTON_H */