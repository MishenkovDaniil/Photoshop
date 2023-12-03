#ifndef PLUG_TRANSFORM_STACK_H
#define PLUG_TRANSFORM_STACK_H

#include "plug_transform.h"
#include "../graphic_structures/vector/vector.h"
#include "../vector.h"


namespace plug {
class TransformStack {
public:
  TransformStack() {}

  virtual ~TransformStack() {}

  /**
   * \brief Enter a transform to the top of the stack and pushes the new
   * transform onto the top of the stack
   */
  virtual void enter(const Transform &transform) = 0;

  /**
   * \brief Removes a transform from the top of the stack
   */
  virtual void leave() = 0;

  /**
   * \brief Gets stack top transform
   */
  virtual Transform top() const = 0;

  /**
   * \brief Converts local position to on-screen position using top transform
   */
  virtual Vec2d apply(const Vec2d &vec) const = 0;

  /**
   * \brief Converts on-screen position to local position using top transform
   */
  virtual Vec2d restore(const Vec2d &vec) const = 0;
};
} // namespace plug
// namespace plug
// {
//     class TransformStack 
//     {
//         M_vector<Transform> transform_stack = M_vector<Transform> (Transform (-1, -1));
//     public:
//         virtual ~TransformStack () = default;
//         virtual void enter (const Transform &transform) = 0;
//         virtual void leave  () = 0;
//         virtual Transform top () const = 0;
//         virtual int get_size () const = 0;
//         virtual Vec2d apply (const Vec2d &vector) const = 0; 
//         virtual Vec2d restore (const Vec2d &vector) const = 0;
//     };
// }

#endif /* PLUG_TRANSFORM_STACK_H */