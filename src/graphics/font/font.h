#ifndef FONT_H
#define FONT_H

#include <SFML/Graphics.hpp>

class Text;

class Font
{
    sf::Font font;
public:
    Font () = default;
    ~Font () = default;

    bool loadFromFile (const char *pathname) {return font.loadFromFile (pathname);};
    friend Text;
};

#endif /* FONT_H */