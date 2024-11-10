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

/// @brief  Create a polygon with `nbSides` vertices (0, 0), (1, 0)...
/// @param nbSides
Polygon::Polygon(int nbSides) {
    init(nbSides);
    initGL();
    log(GREEN "created" RESET ".");
}

/// @brief Create a polygon with `nbSides` vertices `a`, `b`...
/// @param a
/// @param b
/// @param nbSides
Polygon::Polygon(const vec2& a, const vec2& b, int nbSides) {
    init(nbSides);
    vec2 diff = b - a;
    mat2x3 transform = mat2x3(diff.x, -diff.y, a.x, diff.y, diff.x, a.y);
    for (int i = 0; i < nbSides; i++) {
        points[i] = transpose(transform) * vec3(points[i], 1.0f);
    }
    initGL();
    log(GREEN "created" RESET ".");
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

/// @brief Render the polygon using `shaderProgram` and `drawingMode`
/// @param program
void Polygon::render(unsigned shaderProgram, GLenum drawingMode) const {
    int colorUniform = glGetUniformLocation(shaderProgram, "color");
    glUniform3fv(colorUniform, 1, value_ptr(color));
    glBindVertexArray(vao);
    glDrawArrays(drawingMode, 0, points.size());
}

void Polygon::init(int nbSides) {
    assert(nbSides >= 3);
    color = nextColor(50);
    points.reserve(nbSides);
    vec2 xy = vec2(-0.0f);
    for (int i = 0; i < nbSides; i++) {
        points.push_back(xy);
        xy += vec2(cos(2.0 * i * pi<double>() / nbSides),
                   sin(2.0 * i * pi<double>() / nbSides));
    }
}

void Polygon::initGL() {
    glGenBuffers(1, &vbo);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points[0]) * points.size(),
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
    std::clog << "Polygon (" << points.size() << " sides) was " << log
              << std::endl;
}