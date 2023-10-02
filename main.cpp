#include <iostream>
#include <cassert>

#include "widget/widget.h"
#include "window/window.h"

static const int FULLSCREEN_WIDTH = 1920;
static const int FULLSCREEN_HEIGHT = 1080;

int main ()
{
    sf::RenderWindow window (sf::VideoMode (FULLSCREEN_WIDTH, FULLSCREEN_HEIGHT), "w_manager");//, sf::Style::Fullscreen);
    sf::Vector2u window_size = window.getSize ();
    
    sf::RenderTexture render_texture;
    render_texture.create (window_size.x, window_size.y);

    Vector pos (50, 50);
    Color w_color (255, 0, 0);
    Color frame_color (0, 255, 0);
    Window simple_window (800, 800, pos, w_color, frame_color);

    sf::Window fff(sf::VideoMode (500, 500), "window!!");
    
    simple_window.draw (render_texture);
    render_texture.display ();

    sf::Sprite window_sprite;

    while (window.isOpen ())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed || event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
            {
                window.close();
            }
        }
        
        window_sprite.setTexture (render_texture.getTexture ());
        window_sprite.setOrigin (0, 0);
        window_sprite.setPosition (0, 0);
        
        window.clear ();
        // window.draw (fff);
        window.draw (window_sprite);
        fff.display ();
        window.display ();

    }

    return 0;
}