#ifndef TOOLS_H
#define TOOLS_H

#include <iostream>
#include <SFML/Graphics.hpp>

class Filter;

#include "../../widget.h"
#include "../canvas/canvas.h"
#include "../window.h"
#include "../filter/filter.h"
#include "../../text/text.h"
//sf::RTX --> canvas 

class Tool;

static const double DEFAULT_CIRCLE_THICKNESS    = 2.0;
static const double    MIN_CIRCLE_THICKNESS     = 1.0;
static const double DEFAULT_BRUSH_THICKNESS     = 10.0;

// enum Button_state ???
struct Button_state 
{
    bool pressed = false;
    bool released = false;
};

class Tool 
{    
protected:
    Widget *widget_ = nullptr;
    Button_state state_ = {false, false};

public:
    virtual ~Tool () = default;
    virtual void on_main_button         (Button_state &state, Vector &pos, Canvas &canvas) = 0;
    virtual void on_secondary_button    (Button_state &state, Vector &pos, Canvas &canvas) = 0;
    virtual void on_modifier_1          (Canvas &canvas) = 0;
    virtual void on_modifier_2          (Canvas &canvas) = 0;
    virtual void on_modifier_3          (Canvas &canvas) = 0;

    virtual void on_move                (Vector &pos, Canvas &canvas) = 0;
    virtual void on_confirm             (Canvas &canvas) = 0;
    virtual void on_cancel              () = 0;
    virtual void on_released_key        () = 0;

    Widget *get_widget () {return widget_;};
};

class Brush : public Tool
{
    Vector prev_pos = 0;
    bool is_pressed = false;

public:
    Brush ();
    ~Brush ();

    void on_main_button         (Button_state &state, Vector &pos, Canvas &canvas) override;
    void on_secondary_button    (Button_state &state, Vector &pos, Canvas &canvas) override;
    void on_modifier_1          (Canvas &canvas) override;
    void on_modifier_2          (Canvas &canvas) override;
    void on_modifier_3          (Canvas &canvas) override;

    void on_move                (Vector &pos, Canvas &canvas) override;
    void on_confirm             (Canvas &canvas) override;
    void on_cancel              () override;
    void on_released_key        () override {};
};

class Line : public Tool 
{
    sf::Vertex vertex[2];
    Vector latest_pos_ = Vector ();

public:
    Line ();
    ~Line ();

    void on_main_button         (Button_state &state, Vector &pos, Canvas &canvas) override;
    void on_secondary_button    (Button_state &state, Vector &pos, Canvas &canvas) override;
    void on_modifier_1          (Canvas &canvas) override;
    void on_modifier_2          (Canvas &canvas) override;
    void on_modifier_3          (Canvas &canvas) override;

    void on_move                (Vector &pos, Canvas &canvas) override;
    void on_confirm             (Canvas &canvas) override;
    void on_cancel              () override;
    void on_released_key        () override {};
};

// class Polygon : public Tool

class Circle_shape : public Tool
{
    Vector center_;
    Vector last_center_;
    sf::CircleShape circle_ = sf::CircleShape ();
    Vector latest_pos_ = Vector ();
    bool is_on_modifier_1_ = false;

public:
    Circle_shape ();
    ~Circle_shape ();

    void on_main_button         (Button_state &state, Vector &pos, Canvas &canvas) override;
    void on_secondary_button    (Button_state &state, Vector &pos, Canvas &canvas) override;
    void on_modifier_1          (Canvas &canvas) override;
    void on_modifier_2          (Canvas &canvas) override;
    void on_modifier_3          (Canvas &canvas) override;

    void on_move                (Vector &pos, Canvas &canvas) override;
    void on_confirm             (Canvas &canvas) override;
    void on_cancel              () override;
    void on_released_key        () override {is_on_modifier_1_ = false;};
};

class Rect_shape : public Tool 
{
    Vector center_;
    Vector last_center_;
    sf::RectangleShape rect_ = sf::RectangleShape ();
    Color canvas_bg_color = Transparent;
    Vector latest_pos_ = Vector ();
    bool is_on_modifier_1_ = false;

public:
    Rect_shape ();
    ~Rect_shape ();

    void on_main_button         (Button_state &state, Vector &pos, Canvas &canvas) override;
    void on_secondary_button    (Button_state &state, Vector &pos, Canvas &canvas) override;
    void on_modifier_1          (Canvas &canvas) override;
    void on_modifier_2          (Canvas &canvas) override;
    void on_modifier_3          (Canvas &canvas) override;

    void on_move                (Vector &pos, Canvas &canvas) override;
    void on_confirm             (Canvas &canvas) override;
    void on_cancel              () override;
    void on_released_key        () override {is_on_modifier_1_ = false;};

    friend class Text_tool;
};

class Fill : public Tool
{
    uint8_t *pixel_arr_ = 0;
    Vector size_;

    Color fill_color_;
    Color cur_color_;

    sf::Image prev_canvas_img_;
    sf::Texture draw_texture_;
    sf::Sprite draw_sprite_;
    sf::IntRect rect_;

public:
    Fill ();
    ~Fill ();

    void on_main_button         (Button_state &state, Vector &pos, Canvas &canvas) override;
    void on_secondary_button    (Button_state &state, Vector &pos, Canvas &canvas) override;
    void on_modifier_1          (Canvas &canvas) override;
    void on_modifier_2          (Canvas &canvas) override;
    void on_modifier_3          (Canvas &canvas) override;

    void on_move                (Vector &pos, Canvas &canvas) override;
    void on_confirm             (Canvas &canvas) override;
    void on_cancel              () override;
    void on_released_key        () override {};

private:
    void fill_pixels (Vector &pos, Canvas &canvas);
};


class Filter_tool : public Tool 
{
    Filter *filter_ = nullptr;

public:
    Filter_tool (Filter *filter) : filter_(filter) {};
    ~Filter_tool () = default;
    
    void on_main_button         (Button_state &state, Vector &pos, Canvas &canvas) override;
    void on_secondary_button    (Button_state &state, Vector &pos, Canvas &canvas) override;
    void on_modifier_1          (Canvas &canvas) override;
    void on_modifier_2          (Canvas &canvas) override;
    void on_modifier_3          (Canvas &canvas) override;

    void on_move                (Vector &pos, Canvas &canvas) override;
    void on_confirm             (Canvas &canvas) override;
    void on_cancel              () override;
    void on_released_key        () override {};
};

class Text_tool : public Tool
{
    const char buf[512] = {};
    Rect_shape rect_tool = Rect_shape ();
    bool on_rect_ = false;
    Vector latest_pos_ = Vector (0, 0);

public:
    Text_tool ();
    ~Text_tool ();

    void on_main_button         (Button_state &state, Vector &pos, Canvas &canvas) override;
    void on_secondary_button    (Button_state &state, Vector &pos, Canvas &canvas) override;
    void on_modifier_1          (Canvas &canvas) override;
    void on_modifier_2          (Canvas &canvas) override;
    void on_modifier_3          (Canvas &canvas) override;

    void on_move                (Vector &pos, Canvas &canvas) override;
    void on_confirm             (Canvas &canvas) override;
    void on_cancel              () override;
    void on_released_key        () override {if (on_rect_) rect_tool.on_released_key ();};
};

#endif /* TOOLS_H */