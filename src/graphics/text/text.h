#ifndef TEXT_H
#define TEXT_H

#include <SFML/Graphics.hpp>

#include "../font/font.h"

#include "../../graphic_structures/color/color.h"
#include "../../graphic_structures/vector/vector.h"

#include "../drawable.h"

class Text : public Drawable
{
    const Font *font_ = nullptr;

public:
    Text ();
    ~Text ();

    Text *clone () const override {return nullptr;};

    void setString (const char *string);
    void setFont (const Font &font);
    void setFillColor (const plug::Color &color);
    void setCharacterSize (size_t character_size);

    void setPosition (double x, double y);
    void setPosition (const plug::Vec2d pos);

    plug::Vec2d findCharacterPos(size_t character);
    const char *getString () const;
    const Font getFont () const;
    plug::Color getFillColor () const;
    size_t getCharacterSize () const;
    plug::Vec2d getPosition () const;
};

#endif /* TEXT_H */