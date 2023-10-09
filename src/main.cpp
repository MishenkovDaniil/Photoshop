#include <iostream>
#include <cassert>

#include "widget/widget.h"
#include "widget/window/window.h"
#include "widget/button/button.h"
#include "widget/window/menu/menu.h"
#include "widget/widget_manager/widget_manager.h"
#include "widget/window/canvas/canvas.h"
#include "widget/window/master_window/master_window.h"
// #include "widget/window/master_window/master_window.h"

static const int FULLSCREEN_WIDTH = 1920;
static const int FULLSCREEN_HEIGHT = 1080;
bool brush_button_act (Master_window *m_window, sf::Keyboard::Key key);

int main ()
{
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
    
    Master_window main_window (window_size.x - 100, window_size.y - 100, pos, "master");
    Button menu_button (pos + Vector (0, 30), 50, 20, (Button_run_fn)brush_button_act, &main_window, Color (255, 0, 0, 255));
    main_window.add_menu_button (&menu_button);
    // main_window.add_menu_button (&menu_button_2);
    widget_manager.add_widget (&main_window);

    Window child_window (800, 800, Vector (200, 200), "window_1");
    main_window.add_window (&child_window);

    sf::Sprite window_sprite;
    bool status = true;

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

                    status = widget_manager.on_mouse_pressed ((Mouse_key)event.mouseButton.button, pos);
                    break;
                }
                case sf::Event::MouseButtonReleased:
                {
                    Vector pos (event.mouseButton.x, event.mouseButton.y);

                    status = widget_manager.on_mouse_released ((Mouse_key)event.mouseButton.button, pos);
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
        
        if (status)
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

bool brush_button_act (Master_window *m_window, sf::Keyboard::Key key)
{
    assert (m_window);
    static bool run = false;

    run = !run;

    if (run)
    {
        for (int i = 0; i < m_window->windows.size; ++i)
        {
            Window *window = (Window *)list_get (&(m_window->windows), i + 1);
            //  m_window->get_list_elem (i + 1);
            window->canvas_->draw_tool.type = Brush;
            window->canvas_->draw_tool.is_pressed = true;
            window->canvas_->draw_tool.color = Color (255, 0, 255);
        }
        printf ("works\n");
    }
    else 
    {
        for (int i = 0; i < m_window->windows.size; ++i)
        {
            Window *window = (Window *)list_get (&(m_window->windows), i + 1);

            // Window *window = m_window->get_list_elem (i + 1);
            window->canvas_->draw_tool.is_pressed = false;
            window->canvas_->draw_tool.type = Unknown_button;
        }
        printf("end\n");
    }
    return true;
}