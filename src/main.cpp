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

static const int FULLSCREEN_WIDTH = 1920;
static const int FULLSCREEN_HEIGHT = 1080;

bool brush_button_act (void *m_window, void *arg);
//TODO first make contain func/
// !!!
// !!!
// !!!
int main ()
{
    M_vector<Transform> stck (Transform (Vector (0, 0)));
    sf::RenderWindow window (sf::VideoMode (FULLSCREEN_WIDTH, FULLSCREEN_HEIGHT), "w_manager");//, sf::Style::Fullscreen);
    sf::Vector2u window_size = window.getSize ();
    
    sf::RenderTexture render_texture;
    render_texture.create (window_size.x, window_size.y);

    Vector pos (50, 50);
    Color w_color (255, 0, 0);
    Color frame_color (0, 255, 0);

    Widget_manager widget_manager; 

    // Menu menu (pos, 800);
    // Button menu_button_2(pos + Vector (50, 30), 100, 20, nullptr, Color (0, 255, 0, 255));
    // menu.add_button (&menu_button);
    // menu.add_button (&menu_button_2);
    
    Master_window main_window (window_size.x - 50, window_size.y - 50, pos, "master");
    Button menu_button (Vector (0, 0), 50, 20, (Button_run_fn)brush_button_act, &main_window, nullptr, Color (255, 0, 0, 255));
    
    Clock clock_button (pos, 100, 30, nullptr, &main_window, 10, 10, 55, nullptr);
    main_window.add_menu_button (&menu_button);
    widget_manager.add_widget (&main_window);
    widget_manager.add_widget (&clock_button);

    Window child_window (600, 600, Vector (200, 200), "window_1");
    main_window.add_window (&child_window);

    sf::Sprite window_sprite;
    bool status = true;

    sf::Clock clock;
    float delta_clock = 0;

    time_t time_;
    struct tm *timeinfo;
    time (&time_);
    timeinfo = localtime (&time_);

    clock_button.change_time (timeinfo->tm_sec, timeinfo->tm_min, timeinfo->tm_hour);

    // Button_palette ();
    //Button (pos + Vector (0, 30), 50, 20, (Button_run_fn)tool_button_act, (void *)tool_palette, (void *)tool, Color (255, 0, 0, 255))
    //this button in run func just changes current tool in tool_palette to tool; ()it's one for all tool buttons;
    // Tool_palette ();

    while (window.isOpen ())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::KeyPressed:
                {
                    if (event.key.code != sf::Keyboard::Escape)
                    {
                        break;   
                    }
                }
                case sf::Event::Closed:
                {
                    window.close();
                    break;
                }
                case sf::Event::MouseButtonPressed:
                {
                    Vector pos (event.mouseButton.x, event.mouseButton.y);

                    status = widget_manager.on_mouse_pressed ((Mouse_key)event.mouseButton.button, pos, stck);
                    break;
                }
                case sf::Event::MouseButtonReleased:
                {
                    Vector pos (event.mouseButton.x, event.mouseButton.y);

                    status = widget_manager.on_mouse_released ((Mouse_key)event.mouseButton.button, pos, stck);
                    break;
                }
                case sf::Event::MouseMoved:
                {
                    Vector pos (event.mouseMove.x, event.mouseMove.y);

                    status = widget_manager.on_mouse_moved (pos, stck);
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
            widget_manager.render (render_texture, stck);
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

bool brush_button_act (void *m_window_, void *arg)
{
    assert (m_window_);
    static bool run = false;

    run = !run;
    
    Master_window *m_window = (Master_window *)m_window_;

    if (run)
    {
        for (int i = 0; i < m_window->windows.size; ++i)
        {
            Window *window = (Window *)list_get (&(m_window->windows), i + 1);

            window->canvas_->draw_tool.type = Brush;
            window->canvas_->draw_tool.is_pressed = true;
            window->canvas_->draw_tool.color = Color (255, 0, 255);
        }
    }
    else 
    {
        for (int i = 0; i < m_window->windows.size; ++i)
        {
            Window *window = (Window *)list_get (&(m_window->windows), i + 1);

            window->canvas_->draw_tool.is_pressed = false;
            window->canvas_->draw_tool.type = Unknown_button;
        }
    }
    return true;
}