#include "convert_sfml_functions.h"


static sf::Color getSFMLColor (const plug::Color &plug_color)
{
    return sf::Color (plug_color.r, plug_color.g, plug_color.b, plug_color.a);
}

static sf::Vector2f getSFMLVector2f (const plug::Vec2d &plug_vector)
{
    return sf::Vector2f (plug_vector.x, plug_vector.y);
}

static void copyToSFMLVertex (sf::Vertex &sf_vertex, const plug::Vertex &plug_vertex)
{
    sf_vertex.color = plug_vertex.color;
    sf_vertex.position = plug_vertex.position;
    sf_vertex.texCoords = plug_vertex.tex_coords;
}

static void copyToSFMLVertexArray (sf::VertexArray &sf_array, const plug::VertexArray &plug_array)
{
    size_t size = plug_array.getSize ();
    sf_array.setPrimitiveType (static_cast<sf::PrimitiveType>(plug_array.getPrimitive ()));
    sf_array.resize (size);

    for (size_t i = 0; i < size; ++i)
    {
        copyToSFMLVertex (sf_array[i], plug_array[i]);
    }
}

static void copyToSFMLTexture (sf::Texture &sf_texture, const plug::Texture &plug_texture)
{
    sf::Image img;
    img.create(plug_texture.width, plug_texture.height);

    for (size_t it = 0; it < plug_texture.height; it++) 
    {
        for (size_t jt = 0; jt < plug_texture.width; jt++) 
        {
            img.setPixel(jt, it, getSFMLColor(plug_texture.data[it * plug_texture.width + jt]));
        }
    }

    sf_texture.loadFromImage(img);
}