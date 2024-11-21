#ifndef POLYGON_H
#define POLYGON_H

#include <glad/glad.h>
#include <glm/mat3x2.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <memory>
#include <vector>

/// @brief Colored polygon with associated OpenGL VBO and VAO.
/// Non-copyable.
/// Edges and vertices are numbered from 0 to nbSides excluded,
/// in counter-clockwise order.
/// Edge n binds vertex n and vertex (n + 1) % nbSides.
class Polygon : public std::enable_shared_from_this<Polygon> {
    // NOTE nbSides can change during move assignment
    int nbSides{};
    std::vector<glm::vec2> points{};
    glm::mat3x2 position{};
    glm::vec3 color{};
    unsigned vbo{};
    unsigned vao{};
    std::vector<std::weak_ptr<Polygon>> neighbors{};
    void initPoints(int nbSides);
    void initGL();
    void destroyGL(const bool destroyVbo = true, const bool destroyVao = true);
    void log(const char* log) const;

  public:
    Polygon(int nbSides, const glm::vec2& a = glm::vec2(0.0),
            const glm::vec2& b = glm::vec2(0.2, 0.0));
    ~Polygon();
    Polygon(const Polygon&) = delete;
    Polygon& operator=(const Polygon&) = delete;
    Polygon(Polygon&&);
    Polygon& operator=(Polygon&&);
    void positionAt(const glm::vec2& a, const glm::vec2& b);
    void render(const unsigned shaderProgram,
                const GLenum drawingMode = GL_TRIANGLE_FAN) const;
    void highlightEdge(const unsigned shaderProgram, const int edge) const;
    bool bindTo(const std::shared_ptr<Polygon> other, int edge = 0);
    glm::vec2 getFirstVertex() const;
    glm::vec2 getFirstEdge() const;
    void debug() const;
};

static_assert(!std::is_copy_constructible<Polygon>::value);
static_assert(!std::is_copy_assignable<Polygon>::value);

#endif /* POLYGON_H */