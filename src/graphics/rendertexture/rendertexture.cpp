#include "rendertexture.h"

void RenderTexture::draw(const plug::VertexArray& array) 
{
    sf::VertexArray sfml_array;
    copyToSFMLVertexArray (sfml_array, array);
    render_texture_.draw (sfml_array);

    cur_texture = render_texture_.getTexture ();
}

void RenderTexture::draw(const plug::VertexArray& array, const plug::Texture& texture)
{
    sf::VertexArray sfml_array;
    copyToSFMLVertexArray (sfml_array, array);

    sf::Texture sfml_texture;
    copyToSFMLTexture (sfml_texture, texture);

    render_texture_.draw (sfml_array, &sfml_texture);

    cur_texture = render_texture_.getTexture ();
}   

void RenderTexture::draw(const plug::VertexArray& array, Sprite &sprite) 
{
    sf::VertexArray sfml_array;
    copyToSFMLVertexArray (sfml_array, array);
    render_texture_.draw (sfml_array);
    render_texture_.display ();

    cur_texture = render_texture_.getTexture ();
    ((sf::Sprite *)sprite.drawable)->setTexture (cur_texture);
}

void RenderTexture::draw(const plug::VertexArray& array, const plug::Texture& texture, Sprite &sprite)
{
    sf::VertexArray sfml_array;
    copyToSFMLVertexArray (sfml_array, array);

    sf::Texture sfml_texture;
    copyToSFMLTexture (sfml_texture, texture);

    render_texture_.draw (sfml_array, &sfml_texture);
    render_texture_.display ();

    cur_texture = render_texture_.getTexture ();
    ((sf::Sprite *)sprite.drawable)->setTexture (cur_texture);
}   

void RenderTexture::clear(plug::Color color)
{
    render_texture_.clear (getSFMLColor (color));
    cur_texture = render_texture_.getTexture ();
}

void RenderTexture::clear (plug::Color color, Sprite &sprite)
{
    render_texture_.clear (getSFMLColor (color));
    render_texture_.display ();
    
    cur_texture = render_texture_.getTexture ();
    ((sf::Sprite *)sprite.drawable)->setTexture (cur_texture);
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

void RenderTexture::draw (const Drawable &drawable, Sprite &sprite)
{
    assert (drawable.drawable);
    render_texture_.draw (*(drawable.drawable));
    render_texture_.display ();
    
    cur_texture = render_texture_.getTexture ();
    ((sf::Sprite *)sprite.drawable)->setTexture (cur_texture);
}


plug::Texture RenderTexture::getTexture () const
{
    sf::Image img = cur_texture.copyToImage ();
    
    plug::Texture texture (img.getSize ().x, img.getSize ().y);
    memcpy (texture.data, img.getPixelsPtr (), sizeof (plug::Color) * img.getSize ().x * img.getSize ().y);
    
    return texture;
}

plug::Vec2d RenderTexture::getSize () const
{
    sf::Vector2u size = render_texture_.getSize ();
    return plug::Vec2d (size.x, size.y);
}
