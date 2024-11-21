#include "polygon.h"
#include "utils.h"
#include <cassert>
#include <exception>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>

using namespace glm;

/// @brief Create a polygon with `nbSides` vertices `a`, `b`...
/// @param nbSides (should be between 3 and 8 included)
/// @param a defaults to (0.0, 0.0)
/// @param b defaults to (0.2, 0.0)
Polygon::Polygon(int nbSides, const vec2& a, const vec2& b) : nbSides(nbSides) {
    color = nextColor(50);
    initPoints();
    neighbors.resize(nbSides);
    initGL();
    positionAt(a, b);
    log(" was " GREEN "created" RESET ".");
}

/// @brief Bind polygon to edge `edge` on `other`.
/// @param other
/// @param edge defaults to 0
bool Polygon::bindTo(const std::shared_ptr<Polygon> other, int edge) {
    bool success = true;
    try {
        other->neighbors[edge] = shared_from_this();
    } catch (const std::exception& e) {
        logError(e.what());
        success = false;
    }
    neighbors[0] = other;
    positionAt(other->position * vec3(other->points[edge + 1], 1.0),
               other->position * vec3(other->points[edge], 1.0));
    log(" was " YELLOW "bound" RESET ".");
    return success;
}

/// @brief Render the polygon using `shaderProgram` and `drawingMode`, then
/// using `shaderProgram` and `GL_LINE_LOOP`.
/// @param shaderProgram
/// @param drawingMode defaults to `GL_TRIANGLE_FAN`
void Polygon::render(const unsigned shaderProgram,
                     const GLenum drawingMode) const {
    int colorUniform = glGetUniformLocation(shaderProgram, "color");
    glUniform3fv(colorUniform, 1, value_ptr(color));
    int positionUniform = glGetUniformLocation(shaderProgram, "position");
    glUniformMatrix3x2fv(positionUniform, 1, GL_FALSE, value_ptr(position));
    glBindVertexArray(vao);
    glDrawArrays(drawingMode, 0, nbSides);
    glUniform3f(colorUniform, 0.0, 0.0, 0.0);
    glDrawArrays(GL_LINE_LOOP, 0, nbSides);
}

void Polygon::highlightEdge(const unsigned shaderProgram,
                            const int edge) const {
    int colorUniform = glGetUniformLocation(shaderProgram, "color");
    glUniform3fv(colorUniform, 1, value_ptr(cursorColor()));
    int positionUniform = glGetUniformLocation(shaderProgram, "position");
    glUniformMatrix3x2fv(positionUniform, 1, GL_FALSE, value_ptr(position));
    glBindVertexArray(vao);
    glLineWidth(5.0);
    glDrawArrays(GL_LINE_LOOP, edge, 2);
    glLineWidth(1.0);
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
vec2 Polygon::getVertex(const int vertex) const {
    return position * vec3(points[vertex], 1.0);
}

void Polygon::debug() const {
    log(":");
    std::clog << "Position: " << position[2].x << ", " << position[2].y
              << "; Vector: " << position[0].x << ", " << position[0].y
              << std::endl;
    std::clog << "Neighbors:" << std::endl;
    for (auto& neighbor : neighbors) {
        if (neighbor.lock() == nullptr) {
            std::clog << "nullptr" << std::endl;
        } else {
            neighbor.lock()->log("");
        }
    }
    std::clog << std::endl;
}

Polygon::~Polygon() {
    destroyGL();
    log(" was " RED "deleted" RESET ".");
}

/* Polygon::Polygon(Polygon&& other)
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
} */

void Polygon::initPoints() {
    assert(nbSides >= 2 && nbSides <= 8);
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

void Polygon::destroyGL(const bool destroyVbo, const bool destroyVao) {
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