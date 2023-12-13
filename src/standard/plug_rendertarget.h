#ifndef PLUG_RENDERTARGET_H
#define PLUG_RENDERTARGET_H

#include "../graphic_structures/color/color.h"
#include "plug_vertex.h"
#include "plug_texture.h"


namespace plug {

/**
 * @brief Framebuffer object with single color attachment (as texture) and
 * depth-stencil buffer (as RenderBuffer) (just use it as a window wrapper, both
 * SFML and SDL will perform all the required magic by themselves)
 *
 */
class RenderTarget {
public:
  virtual ~RenderTarget(void) = default;

  /**
   * @brief Draw vertex array with specified vertex colors
   *
   * @param array
   */
  virtual void draw(const VertexArray &array) = 0;

  /**
   * @brief Draw vertex array with texture-based coloring
   *
   * @param array
   * @param texture
   */
  virtual void draw(const VertexArray &array, const Texture &texture) = 0;

  /**
   * @brief Fill color attachment with specified color and reset depth and
   * stencil buffers
   *
   * @param color
   */
  virtual void clear(Color color) = 0;

  /**
   * @brief Prepare current render target for rendering raw OpenGL
   *
   * @param[in] active true: bind related framebuffer and push GL states, false:
   * pop saved GL states
   */
  virtual void setActive(bool active) = 0;
};

} // namespace plug

#endif /* PLUG_RENDERTARGET_H */