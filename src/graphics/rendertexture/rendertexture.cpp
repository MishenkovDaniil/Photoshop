#include "rendertexture.h"

void RenderTexture::draw(const plug::VertexArray& array) 
{
    sf::VertexArray sfml_array;
    copyToSFMLVertexArray (sfml_array, array);
    render_texture_.draw (sfml_array);
}

void RenderTexture::draw(const plug::VertexArray& array, const plug::Texture& texture)
{
    sf::VertexArray sfml_array;
    copyToSFMLVertexArray (sfml_array, array);

    sf::Texture sfml_texture;
    copyToSFMLTexture (sfml_texture, texture);

    render_texture_.draw (sfml_array, &sfml_texture);
}   

void RenderTexture::clear(plug::Color color)
{
    render_texture_.clear (getSFMLColor (color));
}

unsigned RenderTexture::getNativeHandle(void) const 
{
    ;
}

void RenderTexture::setActive (bool active)
{
    render_texture_.setActive (active);
}

void RenderTexture::draw (const Drawable &drawable)
{
    assert (drawable.drawable);
    render_texture_.draw (*(drawable.drawable));
}

plug::Texture RenderTexture::getTexture () const
{
    sf::Texture sf_texture = render_texture_.getTexture ();
    sf::Image img = sf_texture.copyToImage ();
    plug::Texture texture (img.getSize ().x, img.getSize ().y, (plug::Color *)(img.getPixelsPtr ()));

    return texture;
}

plug::Vec2d RenderTexture::getSize () const
{
    sf::Vector2u size = render_texture_.getSize ();
    return plug::Vec2d (size.x, size.y);
}
