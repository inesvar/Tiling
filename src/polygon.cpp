#include "polygon.h"
#include "color.h"
#include <cassert>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>

#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define BLUE "\033[1;34m"
#define RESET "\033[0m"

using namespace glm;

/// @brief Create a polygon with `nbSides` vertices `a`, `b`...
/// @param nbSides (should be greater or equal to 3)
/// @param a defaults to (0.0, 0.0)
/// @param b defaults to (0.0, 1.0)
Polygon::Polygon(int nbSides, const vec2& a, const vec2& b) {
    assert(nbSides >= 3);
    color = nextColor(50);
    points.resize(nbSides);
    initGL();
    positionAt(a, b);
    log(GREEN "created" RESET ".");
}

/// @brief Render the polygon using `shaderProgram` and `drawingMode`.
/// @param shaderProgram
/// @param drawingMode defaults to `GL_TRIANGLE_FAN`
void Polygon::render(unsigned shaderProgram, GLenum drawingMode) const {
    int colorUniform = glGetUniformLocation(shaderProgram, "color");
    glUniform3fv(colorUniform, 1, value_ptr(color));
    glBindVertexArray(vao);
    glDrawArrays(drawingMode, 0, points.size());
}

/// @brief Position the polygon so that vertices are on `a`, `b`...
/// @param a
/// @param b
/// @param bufferDrawingMode defaults to `GL_STATIC_DRAW`
void Polygon::positionAt(const vec2& a, const vec2& b,
                         GLenum bufferDrawingMode) {
    vec2 vertex = a;
    vec2 firstEdge = b - a;
    mat2 edge = mat2(firstEdge.x, -firstEdge.y, firstEdge.y, firstEdge.x);

    for (unsigned i = 0; i < points.size(); i++) {
        points[i] = vertex;
        vertex +=
            transpose(edge) * vec2(cos(2.0 * i * pi<double>() / points.size()),
                                   sin(2.0 * i * pi<double>() / points.size()));
    }

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points[0]) * points.size(),
                 (const void*)&points[0], bufferDrawingMode);
}

Polygon::~Polygon() {
    destroyGL();
    log(RED "deleted" RESET ".");
}

Polygon::Polygon(Polygon&& other)
    : points(std::move(other.points)), color(std::move(other.color)),
      vbo(std::move(other.vbo)), vao(std::move(other.vao)) {
    other.vao = 0;
    other.vbo = 0;
    log(BLUE "created using move" RESET ".");
}

Polygon& Polygon::operator=(Polygon&& other) {
    if (&other != this) {
        points = std::move(other.points);
        color = std::move(other.color);
        destroyGL((vbo != other.vbo), (vao != other.vao));
        vbo = std::move(other.vbo);
        vao = std::move(other.vao);
        other.vao = 0;
        other.vbo = 0;
    }
    log(BLUE "assigned using move" RESET ".");
    return *this;
}

void Polygon::initGL() {
    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &vao);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);
}

void Polygon::destroyGL(bool destroyVbo, bool destroyVao) {
    if (destroyVbo && glIsBuffer(vbo)) {
        glDeleteBuffers(1, &vbo);
    }
    if (destroyVao && glIsVertexArray(vao)) {
        glDeleteVertexArrays(1, &vao);
    }
}

void Polygon::log(const char* log) const {
    std::clog << "Polygon (" << points.size() << " sides) was " << log
              << std::endl;
}