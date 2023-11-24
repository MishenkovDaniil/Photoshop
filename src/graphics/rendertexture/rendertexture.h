#ifndef RENDERTEXTURE_H
#define RENDERTEXTURE_H

#include <SFML/Graphics.hpp>

#include "../convert_sfml_functions.h"
#include "../../standard/plug_rendertarget.h"
#include "../rectangleshape/rectangleshape.h"
#include "../circleshape/circleshape.h"
#include "../shape.h"

class RenderTexture : public plug::RenderTarget
{
    sf::RenderTexture render_texture_;
public:

    RenderTexture () = default;
    ~RenderTexture() = default;

    void draw (const plug::VertexArray& array) override;
    void draw (const plug::VertexArray& array, const plug::Texture& texture) override;
    
    void draw (const Shape &shape);

    void clear (plug::Color color) override;
    unsigned getNativeHandle(void) const override;
    void setActive (bool active) override;
};

#endif /* RENDERTEXTURE_H */