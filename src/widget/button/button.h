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
    Button (Vector lh_corner, int width, int height, Button_run_fn func, void *controlled_widget,
            void *arg = nullptr, Color fill_color = Black, int run_mask = RELEASE_BUTTON);
    
    virtual ~Button () {};

    Transform &get_transform () {return transform_;};

    bool contains (double x, double y) const;
    Button_run_fn get_func          ()const {return run_fn_;};
    Color         get_fill_color    ()const {return fill_color_;};
    bool          get_status        ()const {return is_pressed_;}; //may be instead made bool is_pressed (...) const {...};
    int           get_width         ()const {return width_;};
    int           get_height        ()const {return height_;};

    void          set_arg           (void *arg) {arg_ = arg;};
    
    bool run ();
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

class Texture_button : public Button
{
protected:
    sf::Texture *cur_texture_ = nullptr;
    sf::Texture &pressed_texture_;
    sf::Texture &released_texture_;
    sf::Sprite *sprite_ = nullptr;

public:
    Texture_button (Vector lh_corner, int width, int height, sf::Texture &pressed, sf::Texture &released, 
                    Button_run_fn func, void *controlled_widget, void *arg = nullptr, int run_mask = RELEASE_BUTTON);
    ~Texture_button () override;

    void render (sf::RenderTarget &target, M_vector<Transform> &transform_stack) override;
    bool on_mouse_pressed  (Mouse_key mouse_key, Vector &pos) override;
    bool on_mouse_released (Mouse_key mouse_key, Vector &pos) override;
    bool on_mouse_moved    (Vector &new_pos)                  override;
    bool on_keyboard_pressed  (Keyboard_key key)              override;
    bool on_keyboard_released (Keyboard_key key)              override;
};

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

#endif /* BUTTON_H */