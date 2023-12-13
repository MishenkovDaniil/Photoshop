#ifndef PLUG_TRANSFORM_H
#define PLUG_TRANSFORM_H

#include "../graphic_structures/vector/vector.h"


namespace plug {
class Transform {
public:
  /**
   * \brief Creates transform with the given offset and scale
   */
  explicit Transform(const Vec2d &offset_ = Vec2d(0, 0),
                     const Vec2d &scale_ = Vec2d(1, 1))
      : offset(offset_), scale(scale_) {}

  /**
   * \brief Returns offset
   */
  Vec2d getOffset() const { return offset; }

  /**
   * \brief Sets offset
   */
  void setOffset(const Vec2d &offset_) { offset = offset_; }

  /**
   * \brief Returns scale
   */
  Vec2d getScale() const { return scale; }

  /**
   * \brief Sets scale
   */
  void setScale(const Vec2d &scale_) { scale = scale_; }

  /**
   * \brief Converts local position to on-screen position
   */
  Vec2d apply(const Vec2d &vec) const { return vec * scale - offset; }

  /**
   * \brief Converts on-screen position to local position
   */
  Vec2d restore(const Vec2d &vec) const { return (vec + offset) / scale; }

  /**
   * \brief Combines parent transform with this one
   */
  Transform combine(const Transform &parent_transform) const {
    return Transform(parent_transform.getOffset() +
                         offset * parent_transform.getScale(),
                     scale * parent_transform.getScale());
  }

private:
  Vec2d offset;
  Vec2d scale;
};
} // namespace plug

#endif /* PLUG_TRANSFORM_H */