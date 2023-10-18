#ifndef TOOLS_H
#define TOOLS_H

#include <iostream>
#include <SFML/Graphics.hpp>

#include "../../widget.h"
#include "../canvas/canvas.h"
#include "../window.h"

//sf::RTX --> canvas 
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
    virtual void on_modifier_1          (Button_state &state, Vector &pos, Canvas &canvas) = 0;
    virtual void on_modifier_2          (Button_state &state, Vector &pos, Canvas &canvas) = 0;
    virtual void on_modifier_3          (Button_state &state, Vector &pos, Canvas &canvas) = 0;

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
    void on_modifier_1          (Button_state &state, Vector &pos, Canvas &canvas) override;
    void on_modifier_2          (Button_state &state, Vector &pos, Canvas &canvas) override;
    void on_modifier_3          (Button_state &state, Vector &pos, Canvas &canvas) override;

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
    void on_modifier_1          (Button_state &state, Vector &pos, Canvas &canvas) override;
    void on_modifier_2          (Button_state &state, Vector &pos, Canvas &canvas) override;
    void on_modifier_3          (Button_state &state, Vector &pos, Canvas &canvas) override;

    void on_move                (Vector &pos, Canvas &canvas) override;
    void on_confirm             (Vector &pos, Canvas &canvas) override;
    void on_cancel              (Canvas &canvas) override;
};

// class Polygon : public Tool

class Circle_shape : public Tool
{
    Vector center_;
    sf::CircleShape circle_ = sf::CircleShape ();

public:
    Circle_shape ();
    ~Circle_shape ();

    void on_main_button         (Button_state &state, Vector &pos, Canvas &canvas) override;
    void on_secondary_button    (Button_state &state, Vector &pos, Canvas &canvas) override;
    void on_modifier_1          (Button_state &state, Vector &pos, Canvas &canvas) override;
    void on_modifier_2          (Button_state &state, Vector &pos, Canvas &canvas) override;
    void on_modifier_3          (Button_state &state, Vector &pos, Canvas &canvas) override;

    void on_move                (Vector &pos, Canvas &canvas) override;
    void on_confirm             (Vector &pos, Canvas &canvas) override;
    void on_cancel              (Canvas &canvas) override;
    // Vector centre_pos   = 0; //lh_pos instead maybe
    // double radius = 0;

    // int point_num       = 0;
    // sf::Vertexarray (with point num vertexes);

    // bool apply_begin (sf::RenderTexture texture, Vector &pos)
    // {
    //     centre_pos = pos;
    // }
    // bool apply_continue (sf::RenderTexture texture, Vector &pos)
    // {
    //     sf::circleshape circle(centre_pos)
    //     circle.radius = ro (pos - center_pos);
    //     buffer_texture.draw (circle);
    // }
    // bool apply_end      (sf::RenderTexture texture, Vector &pos)
    // {
    //     apply_continue (texture, pos);
    // }
    // bool apply_cancel      (sf::RenderTexture texture, Vector &pos)
    // {
    //     buffer.clear (0, 0,0 ,0);
    // }
};

// class Rect_shape_tool : public Tool 
// {
//     Vector lh_pos;

//     bool apply_begin (sf::RenderTexture texture, Vector &pos)
//     {
//         lh_pos = pos;
//     }
//     bool apply_continue (sf::RenderTexture texture, Vector &pos)
//     {
//         width = abs (pos.x - lh_pos.x);
//         height = abs (pos.y - lh_pos.y);
        
//         sf::rectshape rect(Vector (width, height));
//         buffer_texture.draw (rect);
//     }
//     bool apply_end      (sf::RenderTexture texture, Vector &pos)
//     {
//         apply_continue (texture, pos);
//     }
//     bool apply_cancel      (sf::RenderTexture texture, Vector &pos)
//     {
//         buffer.clear (0, 0,0 ,0);
//     }
// }

#endif /* TOOLS_H */