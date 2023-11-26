#include "texture_widget.h"
#include "../../graphics/rendertexture/rendertexture.h"
#include "../../graphics/sprite/sfml_texture.h"

void M_render_texture::render (plug::RenderTarget &target, plug::TransformStack &transform_stack)
{
    plug::Vec2d lh_pos = transform_stack.top ().getOffset ();

    Sprite sprite;
    sprite.setTexture (texture_.getTexture ());
    sprite.setPosition (lh_pos);
    
    ((RenderTexture &)target).draw (sprite);
}