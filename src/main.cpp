#include <iostream>
#include <cassert>
#include <ctime>

#include "widget/widget.h"
#include "widget/window/window.h"
#include "widget/button/button.h"
#include "widget/window/menu/menu.h"
#include "widget/widget_manager/widget_manager.h"
#include "widget/window/canvas/canvas.h"
#include "widget/window/master_window/master_window.h"
#include "widget/button/clock/clock.h"
#include "widget/window/tools/tools.h"
#include "widget/window/tools/palette/palette.h"
#include "widget/button/palette/palette.h"
#include "widget/window/filter/filter.h"

static const char *brush_img = "resources/imgs/brush.png";
static const char *line_img = "resources/imgs/line.png";
static const char *circle_img = "resources/imgs/circle.png";
static const char *fill_img = "resources/imgs/fill.png";

static const int FULL_SCREEN_WIDTH = 1920;
static const int FULL_SCREEN_HEIGHT = 1080;

//TODO first make contain func/
// !!!
// !!!
// !!!
int main ()
{
    sf::RenderWindow window (sf::VideoMode (FULL_SCREEN_WIDTH, FULL_SCREEN_HEIGHT), "w_manager");//, sf::Style::Fullscreen);
    sf::Vector2u window_size = window.getSize ();
    
    sf::RenderTexture render_texture;
    render_texture.create (window_size.x, window_size.y);

    Vector pos (50, 50);

    Widget_manager widget_manager; 

    Tool_palette palette;
    Master_window main_window (window_size.x, window_size.y, Vector (0, 0), "master");
    
    Clock clock_button (pos, 100, 30, nullptr, &main_window, 10, 10, 55, nullptr);
    widget_manager.add_widget (&main_window);
    widget_manager.add_widget (&clock_button);

    Window child_window (600, 600, Vector (200, 200), "window_1");
    Window child_window_2 (600, 600, Vector (600, 600), "window_2");
    main_window.add_window (&child_window);
    main_window.add_window (&child_window_2);

    sf::Sprite window_sprite;
    bool status = true;

    sf::Clock clock;
    float delta_clock = 0;

    time_t time_;
    struct tm *time_info;
    time (&time_);
    time_info = localtime (&time_);

    clock_button.change_time (time_info->tm_sec, time_info->tm_min, time_info->tm_hour);

    Button_palette button_palette (Vector (1650, 50), 200, 200, &palette);
    
    class Brush brush_tool; 
    Line line_tool; 
    Circle_shape circle_tool; 
    Fill fill_tool; 
    
    Light_filter light_incr (10);
    Light_filter light_decr (-10);
    Saturation_filter saturation_incr (10);
    Saturation_filter saturation_decr (-10);

    Filter_tool light_incr_tool (&light_incr);
    Filter_tool light_decr_tool (&light_decr);
    Filter_tool saturation_incr_tool (&saturation_incr);
    Filter_tool saturation_decr_tool (&saturation_decr);

    sf::Texture brush_pressed_texture;
    sf::Texture line_pressed_texture;
    sf::Texture fill_pressed_texture;
    sf::Texture circle_pressed_texture;
    sf::Texture brush_released_texture;
    sf::Texture line_released_texture;
    sf::Texture fill_released_texture;
    sf::Texture circle_released_texture;

    circle_released_texture.loadFromFile (circle_img);
     circle_pressed_texture.loadFromFile (circle_img);
     brush_released_texture.loadFromFile (brush_img);
      brush_pressed_texture.loadFromFile (brush_img);
      line_released_texture.loadFromFile (line_img);
      fill_released_texture.loadFromFile (fill_img);
       line_pressed_texture.loadFromFile (line_img);
       fill_pressed_texture.loadFromFile (fill_img);

    Pair light_incr_args = Pair((void *)&palette, &light_incr_tool);
    Pair light_decr_args = Pair((void *)&palette, &light_decr_tool);
    Pair saturation_incr_args = Pair((void *)&palette, &saturation_incr_tool);
    Pair saturation_decr_args = Pair((void *)&palette, &saturation_decr_tool);

    Button red_button    (Vector (0, 160), 20, 20, color_button_run_fn, (void *)&main_window, (void *)&Red, Red, PRESS_BUTTON);
    Button blue_button   (Vector (20, 160), 20, 20, color_button_run_fn, (void *)&main_window, (void *)&Blue, Blue, PRESS_BUTTON);
    Button green_button  (Vector (40, 160), 20, 20, color_button_run_fn, (void *)&main_window, (void *)&Green, Green, PRESS_BUTTON);
    Button white_button  (Vector (60, 160), 20, 20, color_button_run_fn, (void *)&main_window, (void *)&White, White, PRESS_BUTTON);
    Button black_button  (Vector (80, 160), 20, 20, color_button_run_fn, (void *)&main_window, (void *)&Black, Black, PRESS_BUTTON);
    Button cyan_button   (Vector (0, 180), 20, 20, color_button_run_fn, (void *)&main_window, (void *)&Cyan, Cyan, PRESS_BUTTON);
    Button purple_button (Vector (20, 180), 20, 20, color_button_run_fn, (void *)&main_window, (void *)&Purple, Purple, PRESS_BUTTON);
    Button yellow_button (Vector (40, 180), 20, 20, color_button_run_fn, (void *)&main_window, (void *)&Yellow, Yellow, PRESS_BUTTON);
    Button brown_button  (Vector (60, 180), 20, 20, color_button_run_fn, (void *)&main_window, (void *)&Brown, Brown, PRESS_BUTTON);
    Button maroon_button (Vector (80, 180), 20, 20, color_button_run_fn, (void *)&main_window, (void *)&Maroon, Maroon, PRESS_BUTTON);
    
    Texture_button brush_button  (Vector (0, 0), 50, 50, brush_pressed_texture, brush_released_texture, tool_run_fn, (void *)&main_window, (void *)&brush_tool, PRESS_BUTTON);
    Texture_button tool_button   (Vector (50, 0), 50, 50, line_pressed_texture, line_released_texture, tool_run_fn, (void *)&main_window, (void *)&line_tool, PRESS_BUTTON);
    Texture_button circle_button (Vector (100, 0), 50, 50, circle_pressed_texture, circle_released_texture, tool_run_fn, (void *)&main_window, (void *)&circle_tool, PRESS_BUTTON);
    Texture_button fill_button   (Vector (150, 0), 50, 50, fill_released_texture, fill_released_texture, tool_run_fn, (void *)&main_window, (void *)&fill_tool, PRESS_BUTTON);
    
    String_button light_incr_tool_button (Vector (0, 0),  60, 20, "light incr", Purple, Purple, tool_run_fn, (void *)&main_window, (void *)&light_incr_args, PRESS_BUTTON);
    String_button light_decr_tool_button (Vector (60, 0), 60, 20, "light decr", Purple, Purple, tool_run_fn, (void *)&main_window, (void *)&light_decr_args, PRESS_BUTTON);
    String_button saturation_incr_tool_button (Vector (120, 0),  60, 20, "satur incr", Purple, Purple, tool_run_fn, (void *)&main_window, (void *)&saturation_incr_args, PRESS_BUTTON);
    String_button saturation_decr_tool_button (Vector (180, 0), 60, 20, "satur decr", Purple, Purple, tool_run_fn, (void *)&main_window, (void *)&saturation_decr_args, PRESS_BUTTON);
    
    // main_window.add_menu_button (&filter_tool_button);

    main_window.add_menu_button (&light_incr_tool_button);
    main_window.add_menu_button (&light_decr_tool_button);
    main_window.add_menu_button (&saturation_incr_tool_button);
    main_window.add_menu_button (&saturation_decr_tool_button);
    
    button_palette.add_tool_button (&red_button);
    button_palette.add_tool_button (&blue_button);
    button_palette.add_tool_button (&green_button);
    button_palette.add_tool_button (&white_button);
    button_palette.add_tool_button (&black_button);
    button_palette.add_tool_button (&cyan_button);
    button_palette.add_tool_button (&purple_button);
    button_palette.add_tool_button (&yellow_button);
    button_palette.add_tool_button (&brown_button);
    button_palette.add_tool_button (&maroon_button);

    button_palette.add_tool_button (&brush_button);
    button_palette.add_tool_button (&tool_button);
    button_palette.add_tool_button (&circle_button);
    button_palette.add_tool_button (&fill_button);
    
    widget_manager.add_widget (&button_palette);
    //Button (pos + Vector (0, 30), 50, 20, (Button_run_fn)tool_button_act, (void *)tool_palette, (void *)tool, Color (255, 0, 0, 255))
    //this button in run func just changes current tool in tool_palette to tool; ()it's one for all tool buttons;

    while (window.isOpen ())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::KeyPressed:
                {
                    if (event.key.code == sf::Keyboard::Escape)
                    {
                        status = widget_manager.on_keyboard_pressed (Escape); // tool must return  bool
                        if (status)
                            break;
                    }
                    else 
                        break;
                }
                case sf::Event::Closed:
                {
                    window.close();
                    break;
                }
                case sf::Event::MouseButtonPressed:
                {
                    Vector pos (event.mouseButton.x, event.mouseButton.y);

                    status = widget_manager.on_mouse_pressed ((Mouse_key)event.mouseButton.button, pos);
                    break;
                }
                case sf::Event::MouseButtonReleased:
                {
                    Vector pos (event.mouseButton.x, event.mouseButton.y);

                    status = widget_manager.on_mouse_released ((Mouse_key)event.mouseButton.button, pos);
                    break;
                }
                case sf::Event::MouseMoved:
                {
                    Vector pos (event.mouseMove.x, event.mouseMove.y);

                    status = widget_manager.on_mouse_moved (pos);
                    break;
                }
                default:
                {
                    break;
                }
            }
        }       
        // window_sprite.setOrigin (0, 0);
        // window_sprite.setPosition (0, 0);
        float delta_time = clock.getElapsedTime ().asSeconds();
        widget_manager.on_time (delta_clock + delta_time);

        delta_clock = clock.getElapsedTime ().asSeconds() - delta_time;
        clock.restart ();

        // if (status)
        {
            widget_manager.render (render_texture);
            render_texture.display ();
            window_sprite.setTexture (render_texture.getTexture ());

            window.clear ();
            window.draw (window_sprite);
            window.display ();
            status = false;
        }
    }

    return 0;
}