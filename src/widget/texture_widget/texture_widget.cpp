#include "texture_widget.h"

void M_render_texture::render (sf::RenderTarget &target, M_vector<Transform> &transform_stack)
{
    Vector lh_pos = transform_stack.top ().offset_;

    sf::Sprite sprite;
    sprite.setTexture (texture_.getTexture ());
    sprite.setPosition (lh_pos);
    
    target.draw (sprite);
}