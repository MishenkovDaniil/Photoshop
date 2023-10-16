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
class Tool_palette;
class Button_palette;

#include "../widget.h"
#include "../window/window.h"
#include "../../graphic_structures/color/color.h"
#include "../../graphic_structures/point/point.h"
#include "../../graphic_structures/vector/vector.h"

static const int START_CAPACITY = 10;
static const Color Black = Color (0, 0, 0, 255);

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
    Vector press_pos_;
    Color fill_color_;

    Button_run_fn run_fn_ = nullptr;
    void *controlled_widget_ = nullptr;
    void *arg_ = nullptr;
    
    int run_mask_ = 0;

    Transform transform_;

public:
    Button () {}; //TODO: make button_create for this constructor case
    Button (Vector lh_corner, int width, int height, Button_run_fn func, void *controlled_widget, void *arg = nullptr, Color fill_color = Black, int run_mask = RELEASE_BUTTON) :
            transform_ (Transform (lh_corner)),
            width_ (width),
            height_ (height),
            run_fn_ (func),
            fill_color_ (fill_color),
            controlled_widget_ (controlled_widget),
            run_mask_ (run_mask),
            arg_ (arg), 
            press_pos_ (Vector ()) {};
    
    virtual ~Button () {};

    Transform &get_transform () {return transform_;};

    bool contains (double x, double y) const;
    Button_run_fn get_func          ()const {return run_fn_;};
    Color         get_fill_color    ()const {return fill_color_;};
    bool          get_status        ()const {return is_pressed_;}; //may be instead made bool is_pressed (...) const {...};
    int           get_width         ()const {return width_;};
    int           get_height        ()const {return height_;};

    void          set_arg           (void *arg) {arg_ = arg;};
    
    // virtual void update (bool is_pressed) = 0;
    virtual bool run ();//not virtual  maybe
    void render (sf::RenderTarget &target, M_vector<Transform> &transform_stack)                    override;
    bool on_mouse_pressed  (Mouse_key mouse_key, Vector &pos) override;
    bool on_mouse_released (Mouse_key mouse_key, Vector &pos) override;
    bool on_mouse_moved    (Vector &new_pos)                  override;
    bool on_keyboard_pressed  (Keyboard_key key)              override;
    bool on_keyboard_released (Keyboard_key key)              override;
    bool on_time (float delta_sec)                            override;

    friend Scrollbar;
    friend Menu;
    friend Button_palette;
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