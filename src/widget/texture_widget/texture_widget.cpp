#include "texture_widget.h"
#include "../../graphics/rendertexture/rendertexture.h"
#include "../../graphics/sprite/sfml_texture.h"

void M_render_texture::draw (plug::TransformStack &transform_stack, plug::RenderTarget &target)
{
    plug::Vec2d lh_pos = transform_stack.top ().getOffset ();

    static plug::VertexArray array (plug::Points, 1);
    array[0] = {plug::Vec2d (), plug::Vec2d (), plug::Transparent};

    if (is_changed_)
    {
        texture_.draw (array, cur_sprite);
        is_changed_ = false;
    }

    cur_sprite.setPosition (lh_pos);
    // const plug::Texture *texture = cur_sprite.getTexture ();
    // static plug::VertexArray array (plug::Quads, 4);
    // if (texture)
    // {
    //     init_vertices (array, texture); 

    //     target.draw (array, *texture);
    // }

    ((RenderTexture &)target).draw (cur_sprite);
}

void M_render_texture::init_vertices (plug::VertexArray &vertices, const plug::Texture *texture)
{
    plug::Vec2d size (texture->width, texture->height);

    vertices[0].position = plug::Vec2d (0, 0);
    vertices[1].position = plug::Vec2d (0, size.y);
    vertices[2].position = plug::Vec2d (size.x, size.y);
    vertices[3].position = plug::Vec2d (size.x, 0);

    vertices[0].color = plug::White;
    vertices[1].color = plug::White;
    vertices[2].color = plug::White;
    vertices[3].color = plug::White;
    
    vertices[0].tex_coords = plug::Vec2d (0, 0);
    vertices[1].tex_coords = plug::Vec2d (0, size.y - 1);
    vertices[2].tex_coords = plug::Vec2d (size.x - 1, size.y - 1);
    vertices[3].tex_coords = plug::Vec2d (size.x - 1, 0);
}
