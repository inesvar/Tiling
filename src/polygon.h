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
class Polygon /* : public std::enable_shared_from_this<Polygon> */ {
    int nbSides;
    std::vector<glm::vec2> points{};
    glm::mat3x2 position{};
    glm::vec3 color{};
    unsigned vbo{};
    unsigned vao{};
    std::vector<std::weak_ptr<Polygon>> neighbors{};
    void initPoints(int nbSides);
    void initGL();
    void destroyGL(bool destroyVbo = true, bool destroyVao = true);
    void log(const char* log) const;

  public:
    Polygon(int nbSides, const glm::vec2& a = glm::vec2(0.0),
            const glm::vec2& b = glm::vec2(0.4, 0.0));
    Polygon(int nbSides, std::shared_ptr<Polygon> other, int edge = 0);
    ~Polygon();
    Polygon(const Polygon&) = delete;
    Polygon& operator=(const Polygon&) = delete;
    Polygon(Polygon&& other);
    Polygon& operator=(Polygon&& other);
    void positionAt(const glm::vec2& a, const glm::vec2& b);
    void render(unsigned shaderProgram,
                GLenum drawingMode = GL_TRIANGLE_FAN) const;
    glm::vec2 getFirstVertex() const;
    glm::vec2 getFirstEdge() const;
    void debug() const;
};

#endif /* POLYGON_H */