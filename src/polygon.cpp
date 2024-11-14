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
/// @param nbSides (should be between 3 and 8 included)
/// @param a defaults to (0.0, 0.0)
/// @param b defaults to (1.0, 0.0)
Polygon::Polygon(int nbSides, const vec2& a, const vec2& b) {
    color = nextColor(50);
    initPoints(nbSides);
    neighbors.resize(nbSides);
    initGL();
    positionAt(a, b);
    log(" was " GREEN "created" RESET ".");
}

/// @brief Render the polygon using `shaderProgram` and `drawingMode`, then
/// using `shaderProgram` and `GL_LINE_LOOP`.
/// @param shaderProgram
/// @param drawingMode defaults to `GL_TRIANGLE_FAN`
void Polygon::render(unsigned shaderProgram, GLenum drawingMode) const {
    int colorUniform = glGetUniformLocation(shaderProgram, "color");
    glUniform3fv(colorUniform, 1, value_ptr(color));
    int positionUniform = glGetUniformLocation(shaderProgram, "position");
    glUniformMatrix3x2fv(positionUniform, 1, GL_FALSE, value_ptr(position));
    glBindVertexArray(vao);
    glDrawArrays(drawingMode, 0, nbSides);
    glUniform3f(colorUniform, 0.0, 0.0, 0.0);
    glDrawArrays(GL_LINE_LOOP, 0, nbSides);
}

/// @brief Position the polygon so that vertices are on `a`, `b`...
/// @param a
/// @param b
void Polygon::positionAt(const vec2& a, const vec2& b) {
    vec2 diff = b - a;
    position = mat3x2(diff.x, diff.y, -diff.y, diff.x, a.x, a.y);
    // This transformation is :
    // scaling so that diff is a unit vector,
    // rotating so that diff is (1.0, 0.0),
    // and then translating so that a is (0.0, 0.0).
}

vec2 Polygon::getFirstVertex() const { return position[2]; }
vec2 Polygon::getFirstEdge() const { return position[0] + position[2]; }

Polygon::~Polygon() {
    destroyGL();
    log(" was " RED "deleted" RESET ".");
}

Polygon::Polygon(Polygon&& other)
    : nbSides(other.nbSides), points(std::move(other.points)),
      position(std::move(other.position)), color(std::move(other.color)),
      vbo(std::move(other.vbo)), vao(std::move(other.vao)),
      neighbors(std::move(other.neighbors)) {
    other.vao = 0;
    other.vbo = 0;
    log(" was " BLUE "created using move" RESET ".");
}

Polygon& Polygon::operator=(Polygon&& other) {
    if (&other != this) {
        nbSides = other.nbSides;
        points = std::move(other.points);
        position = std::move(other.position);
        color = std::move(other.color);
        destroyGL((vbo != other.vbo), (vao != other.vao));
        vbo = std::move(other.vbo);
        vao = std::move(other.vao);
        other.vao = 0;
        other.vbo = 0;
        neighbors = std::move(other.neighbors);
    }
    log(" was " BLUE "assigned using move" RESET ".");
    return *this;
}

void Polygon::initPoints(int nbSides) {
    assert(nbSides >= 3 && nbSides <= 8);
    this->nbSides = nbSides;
    points.resize(nbSides + 1);
    vec2 xy = vec2(-0.0f);
    float closingAngle = 2.0 * pi<double>() / nbSides;
    for (int n = 0; n <= nbSides; n++) {
        points[n] = xy;
        xy += rotate(n * closingAngle);
    }
    // Vertex 0 is duplicated at the end of `points`
    // so that the last edge can be fetched like the others
    // (otherwise the two vec2 wouldn't be contiguous).
}

void Polygon::initGL() {
    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &vao);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points[0]) * nbSides,
                 (const void*)&points[0], GL_STATIC_DRAW);

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
    std::clog << "Polygon (" << nbSides << " sides)" << log << std::endl;
}

void Polygon::debug() const {
    log(":");
    std::clog << "Position: " << position[2].x << ", " << position[2].y
              << "; Vector: " << position[0].x << ", " << position[0].y
              << std::endl;
}