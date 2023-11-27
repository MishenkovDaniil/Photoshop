#ifndef CONVERT_SFML_FUNCTIONS_H
#define CONVERT_SFML_FUNCTIONS_H

#include <SFML/Graphics.hpp>

#include "../graphic_structures/color/color.h"
#include "../graphic_structures/vector/vector.h"

#include "../standard/plug_vertex.h"
#include "../standard/plug_texture.h"


sf::Color getSFMLColor (const plug::Color &plug_color);
sf::Vector2f getSFMLVector2f (const plug::Vec2d &plug_vector);

void copyToSFMLVertex (sf::Vertex &sf_vertex, const plug::Vertex &plug_vertex);
void copyToSFMLVertexArray (sf::VertexArray &sf_array, const plug::VertexArray &plug_array);
void copyToSFMLTexture (sf::Texture &sf_texture, const plug::Texture &plug_texture);

#endif /* CONVERT_SFML_FUNCTIONS_H */