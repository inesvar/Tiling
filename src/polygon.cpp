#include "polygon.h"
#include "utils.h"
#include <cassert>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>

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
    log(" was " GREEN "created" RESET ".");
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
    // NOTE contrary to the 1st implementation,
    // this loop can't be computed in parallel
    vec2 vertex = a;
    vec2 firstEdge = b - a;
    float closingAngle = 2 * pi<double>() / points.size();

    for (unsigned n = 0; n < points.size(); n++) {
        points[n] = vertex;
        vertex += rotate(firstEdge, n * closingAngle);
    }

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points[0]) * points.size(),
                 (const void*)&points[0], bufferDrawingMode);
}

vec2 Polygon::getFirstVertex() const { return points[0]; }
vec2 Polygon::getFirstEdge() const { return points[1] - points[0]; }

Polygon::~Polygon() {
    destroyGL();
    log(" was " RED "deleted" RESET ".");
}

Polygon::Polygon(Polygon&& other)
    : points(std::move(other.points)), color(std::move(other.color)),
      vbo(std::move(other.vbo)), vao(std::move(other.vao)) {
    other.vao = 0;
    other.vbo = 0;
    log(" was " BLUE "created using move" RESET ".");
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
    log(" was " BLUE "assigned using move" RESET ".");
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
    std::clog << "Polygon (" << points.size() << " sides)" << log << std::endl;
}

void Polygon::debug() const {
    log(":");
    for (auto point : points) {
        std::clog << "(" << point.x << ", " << point.y << ") ";
    }
    std::clog << std::endl;
}