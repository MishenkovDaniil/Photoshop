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

void RenderTexture::draw (const Shape &shape)
{
    switch ((enum class Shapes)shape.getType ())
    {
        case Shapes::Rectangle:
        {
            render_texture_.draw (((RectangleShape &)shape).sfml_rect);
        }
        case Shapes::Circle:
        {
            render_texture_.draw (((CircleShape &)shape).sfml_circle);
        }
        default:
        {
            fprintf (stderr, "Error: unknown shape type.\n");
            break;
        }
    }
}