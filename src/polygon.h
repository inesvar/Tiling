#ifndef POLYGON_H
#define POLYGON_H

#include <glad/glad.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>

/// @brief Colored polygon with associated OpenGL VBO and VAO.
/// Non-copyable.
class Polygon {
    std::vector<glm::vec2> points{};
    glm::vec3 color{};
    unsigned vbo{};
    unsigned vao{};
    void initGL();
    void destroyGL(bool destroyVbo = true, bool destroyVao = true);
    void log(const char* log) const;

  public:
    Polygon(int nbSides, const glm::vec2& a = glm::vec2(0.0),
            const glm::vec2& b = glm::vec2(0.4, 0.0));
    ~Polygon();
    Polygon(const Polygon&) = delete;
    Polygon& operator=(const Polygon&) = delete;
    Polygon(Polygon&& other);
    Polygon& operator=(Polygon&& other);
    void positionAt(const glm::vec2& a, const glm::vec2& b,
                    GLenum bufferDrawingMode = GL_STATIC_DRAW);
    void render(unsigned shaderProgram,
                GLenum drawingMode = GL_TRIANGLE_FAN) const;
    glm::vec2 getFirstVertex() const;
    glm::vec2 getFirstEdge() const;
    void debug() const;
};

#endif /* POLYGON_H */