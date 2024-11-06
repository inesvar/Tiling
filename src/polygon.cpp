#include "polygon.h"
#include "color.h"
#include <GL/glut.h>
#include <cassert>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>

using namespace glm;

/// @brief  Builds a Polygon with vertices (0, 0), (1, 0)...
/// @param nbSides 
Polygon::Polygon(int nbSides) {
    assert(nbSides >= 3);
    color = nextColor();
    points.reserve(nbSides);
    vec2 xy = vec2(-0.0f);
    for (int i = 0; i < nbSides; i++) {
        points.push_back(xy);
        xy += vec2(cos(2.0 * i * pi<double>() / nbSides),
                   sin(2.0 * i * pi<double>() / nbSides));
    }
    center.x = 0.5f;
    center.y = points[int(nbSides / 2)].y;
}

/// @brief Builds a Polygon with vertices a, b...
/// @param a 
/// @param b 
/// @param nbSides 
Polygon::Polygon(const vec2& a, const vec2& b, int nbSides) : Polygon(nbSides) {
    vec2 diff = b - a;
    mat2x3 transform = mat2x3(diff.x, -diff.y, a.x, diff.y, diff.x, a.y);
    for (int i = 0; i < nbSides; i++) {
        points[i] = transpose(transform) * vec3(points[i], 1.0f);
    }
    center = transpose(transform) * vec3(center, 1.0f);
}

void Polygon::render() const {
    glBegin(GL_TRIANGLE_FAN);
    glColor3fv(value_ptr(color));
    glVertex3fv(value_ptr(center));
    for (const auto& vertex : points) {
        glVertex3fv(value_ptr(vertex));
    }
    glVertex3fv(value_ptr(points[0]));
    glEnd();
}

void displayPolygon() {
    vec2 leftOrigin = vec2(-0.2f, -0.8f);
    vec2 rightOrigin = vec2(0.2f, -0.8f);

    std::vector<Polygon> polygons{};
    for (int i = 8; i > 2; i--) {
        polygons.emplace_back(leftOrigin, rightOrigin, i);
    }

    glClear(GL_COLOR_BUFFER_BIT);
    for (auto polygon : polygons) {
        polygon.render();
    }
    glFlush();
}