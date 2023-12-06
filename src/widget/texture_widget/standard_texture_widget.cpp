#include "standard_texture_widget.h"

void PlugRenderTexture::draw (plug::TransformStack &transform_stack, plug::RenderTarget &target)
{
    size_t size = arrays.get_size ();

    for (size_t i = 0; i < size; ++i)
    {
        const plug::VertexArray &array = arrays[i];
        target.draw (array); 
    }
}