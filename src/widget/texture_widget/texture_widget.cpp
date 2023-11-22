#include "texture_widget.h"

void M_render_texture::render (sf::RenderTarget &target, plug::TransformStack &transform_stack)
{
    plug::Vec2d lh_pos = transform_stack.top ().getOffset ();

    sf::Sprite sprite;
    sprite.setTexture (texture_.getTexture ());
    sprite.setPosition (lh_pos);
    
    target.draw (sprite);
}