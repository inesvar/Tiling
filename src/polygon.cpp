#include "polygon.h"
#include "color.h"
#include <GL/glut.h>
#include <cassert>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>

using namespace glm;

Polygon::Polygon(int nbSides) {
    assert(nbSides >= 3);
    color = nextColor();
    points.reserve(nbSides);
    vec2 xy = vec2(-0.0f);
    for (int i = 0; i < nbSides; i++) {
        points.push_back(xy);
        xy += 0.5f * vec2(cos(2.0 * i * pi<double>() / nbSides),
                          sin(2.0 * i * pi<double>() / nbSides));
    }
}

void Polygon::render(void) const {
    glBegin(GL_POLYGON);
    glColor3fv(value_ptr(color));
    for (const auto& vertex : points) {
        glVertex3fv(value_ptr(vertex));
    }
    glEnd();
}

void displayPolygon(void) {
    std::vector<Polygon> polygons{6, 5, 4, 3};
    glClear(GL_COLOR_BUFFER_BIT);
    for (auto polygon : polygons) {
        polygon.render();
    }
    glFlush();
}