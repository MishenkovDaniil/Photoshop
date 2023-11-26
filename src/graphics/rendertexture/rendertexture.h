#ifndef RENDERTEXTURE_H
#define RENDERTEXTURE_H

#include <SFML/Graphics.hpp>

#include "../convert_sfml_functions.h"
#include "../../standard/plug_rendertarget.h"
#include "../rectangleshape/rectangleshape.h"
#include "../circleshape/circleshape.h"
#include "../drawable.h"

#include "../sprite/sfml_texture.h"

class RenderTexture : public plug::RenderTarget
{
    sf::Texture cur_texture;

public:
    sf::RenderTexture render_texture_;
    RenderTexture () = default;
    virtual ~RenderTexture () = default;

    void create (size_t x, size_t y) {render_texture_.create (x, y);};

    void draw (const plug::VertexArray& array)                                  override;
    void draw (const plug::VertexArray& array, const plug::Texture& texture)    override;
    void clear (plug::Color color)                                              override;
    unsigned getNativeHandle(void) const                                        override;
    void setActive (bool active)                                                override;

    plug::Vec2d getSize () const;
    plug::Texture getTexture () const;

    void display () {render_texture_.display ();};
    
    void draw (const plug::VertexArray& array, Sprite &sprite);
    void draw (const plug::VertexArray& array, const plug::Texture& texture, Sprite &sprite);
    void draw (const Drawable &drawable);
    void draw (const Drawable &drawable, Sprite &sprite);
    void clear (plug::Color color, Sprite &sprite);
};

#endif /* RENDERTEXTURE_H */