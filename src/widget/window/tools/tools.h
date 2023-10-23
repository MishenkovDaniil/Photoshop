#ifndef TOOLS_H
#define TOOLS_H

#include <iostream>
#include <SFML/Graphics.hpp>

class Filter;

#include "../../widget.h"
#include "../canvas/canvas.h"
#include "../window.h"
#include "../filter/filter.h"
//sf::RTX --> canvas 

static const double DEFAULT_CIRCLE_THICKNESS    = 2.0;
static const double    MIN_CIRCLE_THICKNESS     = 1.0;
static const double DEFAULT_BRUSH_THICKNESS     = 10.0;

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
    virtual void on_modifier_1          (Vector &pos, Canvas &canvas) = 0;
    virtual void on_modifier_2          (Vector &pos, Canvas &canvas) = 0;
    virtual void on_modifier_3          (Vector &pos, Canvas &canvas) = 0;

    virtual void on_move                (Vector &pos, Canvas &canvas) = 0;
    virtual void on_confirm             (Vector &pos, Canvas &canvas) = 0;
    virtual void on_cancel              (Canvas &canvas) = 0;
    
    Widget *get_widget () {return widget_;};


    // // virtual bool apply_begin    (sf::RenderTexture &texture, Vector &pos) = 0;
    // // virtual bool apply_continue (sf::RenderTexture &texture, Vector &pos) = 0;
    // // virtual bool apply_end      (sf::RenderTexture &texture, Vector &pos) = 0;
    // // virtual bool apply_cancel   (sf::RenderTexture &texture, Vector &pos) = 0;
    
    // // virtual void draw (sf::RenderTarget &target) = 0;
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
    void on_modifier_1          (Vector &pos, Canvas &canvas) override;
    void on_modifier_2          (Vector &pos, Canvas &canvas) override;
    void on_modifier_3          (Vector &pos, Canvas &canvas) override;

    void on_move                (Vector &pos, Canvas &canvas) override;
    void on_confirm             (Vector &pos, Canvas &canvas) override;
    void on_cancel              (Canvas &canvas) override;
};

class Line : public Tool 
{
    sf::Vertex vertex[2];

public:
    Line ();
    ~Line ();

    void on_main_button         (Button_state &state, Vector &pos, Canvas &canvas) override;
    void on_secondary_button    (Button_state &state, Vector &pos, Canvas &canvas) override;
    void on_modifier_1          (Vector &pos, Canvas &canvas) override;
    void on_modifier_2          (Vector &pos, Canvas &canvas) override;
    void on_modifier_3          (Vector &pos, Canvas &canvas) override;

    void on_move                (Vector &pos, Canvas &canvas) override;
    void on_confirm             (Vector &pos, Canvas &canvas) override;
    void on_cancel              (Canvas &canvas) override;
};

// class Polygon : public Tool

class Circle_shape : public Tool
{
    Vector center_;
    Vector last_center_;
    sf::CircleShape circle_ = sf::CircleShape ();

public:
    Circle_shape ();
    ~Circle_shape ();

    void on_main_button         (Button_state &state, Vector &pos, Canvas &canvas) override;
    void on_secondary_button    (Button_state &state, Vector &pos, Canvas &canvas) override;
    void on_modifier_1          (Vector &pos, Canvas &canvas) override;
    void on_modifier_2          (Vector &pos, Canvas &canvas) override;
    void on_modifier_3          (Vector &pos, Canvas &canvas) override;

    void on_move                (Vector &pos, Canvas &canvas) override;
    void on_confirm             (Vector &pos, Canvas &canvas) override;
    void on_cancel              (Canvas &canvas) override;
};

// class Rect_shape_tool : public Tool 

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
    void on_modifier_1          (Vector &pos, Canvas &canvas) override;
    void on_modifier_2          (Vector &pos, Canvas &canvas) override;
    void on_modifier_3          (Vector &pos, Canvas &canvas) override;

    void on_move                (Vector &pos, Canvas &canvas) override;
    void on_confirm             (Vector &pos, Canvas &canvas) override;
    void on_cancel              (Canvas &canvas) override;

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
    void on_modifier_1          (Vector &pos, Canvas &canvas) override;
    void on_modifier_2          (Vector &pos, Canvas &canvas) override;
    void on_modifier_3          (Vector &pos, Canvas &canvas) override;

    void on_move                (Vector &pos, Canvas &canvas) override;
    void on_confirm             (Vector &pos, Canvas &canvas) override;
    void on_cancel              (Canvas &canvas) override;
    
};

#endif /* TOOLS_H */