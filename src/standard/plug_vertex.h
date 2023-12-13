#ifndef PLUG_VERTEX_H
#define PLUG_VERTEX_H

#include "../graphic_structures/vector/vector.h"
#include "../graphic_structures/color/color.h"

// const double RESIZE_FACTOR = 1.5;


namespace plug {
enum PrimitiveType
    {
        /**
         * @brief Draw vertices as individual points
         *
         */
        Points,

        /**
         * @brief Draw set of unconnected lines (f.e. [0]---[1]   [2]---[3] [4]---[5]
         * ...)
         *
         */
        Lines,

        /**
         * @brief Draw set of connected lines ([0]---[1]---[2]---[3]---[4]-...)
         *
         */
        LineStrip,

        /**
         * @brief Draw set of unconnected triangles (<0 1 2> <3 4 5> <6 7 8> ...)
         *
         */
        Triangles,

        /**
         * @brief Draw a set of connected triangles where each triangle shares its two
         * last vertices with the next one (<0 1 2> <1 2 3> <2 3 4> <3 4 5> ...)
         *
         */
        TriangleStrip,

        /**
         * @brief Draw a set of triangles connected to a central point (<0 1 2> <0 2
         * 3> <0 3 4> <0 4 5> ...)
         *
         */
        TriangleFan,

        /**
         * @brief Draw a set of unconnected quads ([0 1 2 3] [4 5 6 7] [8 9 10 11]
         * ...)
         *
         */
        Quads,

        /**
         * @brief [deprecated] Replaced with LineStrip
         *
         */
        LinesStrip = LineStrip,

        /**
         * @brief [deprecated] Replaced with TriangleStrip
         *
         */
        TrianglesStrip = TriangleStrip,

        /**
         * @brief [deprecated] Replaced with TriangleFan
         *
         */
        TrianglesFan = TriangleFan,
    };

/**
 * @brief Geometrical vertex
 *
 */
class Vertex {
public:
  /**
   * @brief Vertex position in piskels (SFML screen units, equal to 1 pixel if
   * the window is not resizable)
   *
   */
  // Vertex () = default;
  Vec2d position;

  /**
   * @brief Coordinate of the vertex on the texture (from 0 to 1 on each axis)
   *
   */
  Vec2d tex_coords;

  /**
   * @brief Vertex color (can be used to transfer some additional data to the
   * GPU)
   *
   */
  Color color;
};

/**
 * @brief Vertex buffer with defined primitive type and index-based vertex
 * enumeration
 *
 */
class VertexArray {
public:
  VertexArray(PrimitiveType type, size_t size);

  VertexArray(const VertexArray &other);

  VertexArray &operator=(const VertexArray &other);

  ~VertexArray(void);

  /**
   * @brief Get rendering primitive
   *
   * @return PrimitiveType
   */
  PrimitiveType getPrimitive(void) const;

  /**
   * @brief Set rendering primitive
   *
   * @param type
   */
  void setPrimitive(PrimitiveType type);

  /**
   * @brief Get number of vertices in the array
   *
   */
  size_t getSize(void) const;

  /**
   * @brief Resize the array to fit specified number of vertices
   *
   * @param new_size
   */
  void resize(size_t new_size);

  /**
   * @brief Add vertex to the end of the array
   *
   * @param vertex
   */
  void appendVertex(const Vertex &vertex);

  Vertex &operator[](size_t index);

  const Vertex &operator[](size_t index) const;

private:
  PrimitiveType m_type;

  Vertex *m_data;
  size_t m_size;
  size_t m_capacity;

  void reserve(size_t new_capacity);
};

inline VertexArray::VertexArray(PrimitiveType type, size_t size = 0)
    : m_type(type), m_size(size), m_capacity(std::max(size, 1ul)) {
  m_data = new Vertex[m_capacity];
}

inline VertexArray::VertexArray(const VertexArray &other)
    : m_type(other.m_type), m_size(other.m_size), m_capacity(other.m_capacity) {
  m_data = new Vertex[m_capacity];
  for (size_t ind = 0; ind < m_size; ++ind) {
    m_data[ind] = other.m_data[ind];
  }
}

inline VertexArray &VertexArray::operator=(const VertexArray &other) {
  if (this == &other) {
    return *this;
  }

  delete m_data;

  m_type = other.m_type;
  m_size = other.m_size;
  m_capacity = other.m_capacity;

  m_data = new Vertex[m_capacity];
  for (size_t ind = 0; ind < m_size; ++ind) {
    m_data[ind] = other.m_data[ind];
  }

  return *this;
}

inline VertexArray::~VertexArray(void) {
  m_size = m_capacity = -1;
  delete[] m_data;
}

inline PrimitiveType VertexArray::getPrimitive(void) const { return m_type; }

inline void VertexArray::setPrimitive(PrimitiveType type) { m_type = type; }

inline size_t VertexArray::getSize(void) const { return m_size; }

inline void VertexArray::resize(size_t new_size) {
  if (new_size <= m_capacity) {
    m_size = new_size;
    return;
  }

  reserve(new_size);
  m_size = new_size;
}

inline void VertexArray::appendVertex(const Vertex &vertex) {
  if (m_size == m_capacity) {
    reserve(m_capacity * 2);
  }
  assert(m_size < m_capacity);

  m_data[m_size++] = vertex;
}

inline Vertex &VertexArray::operator[](size_t index) {
  assert(index < m_size);
  return m_data[index];
}

inline const Vertex &VertexArray::operator[](size_t index) const {
  assert(index < m_size);
  return m_data[index];
}

inline void VertexArray::reserve(size_t new_capacity) {
  assert(new_capacity > m_capacity);

  Vertex *new_data = new Vertex[new_capacity];
  for (size_t ind = 0; ind < m_size; ++ind) {
    new_data[ind] = m_data[ind];
  }
  delete[] m_data;

  m_data = new_data;
  m_capacity = new_capacity;
}

} // end namespace plug

#endif /* PLUG_VERTEX_H */