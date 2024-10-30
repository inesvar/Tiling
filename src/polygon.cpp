#include "polygon.h"
#include <GL/glut.h>
#include <cassert>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

using namespace glm;

Polygon polygon{3};

Polygon::Polygon(int nbSides) {
    assert(nbSides >= 3);
    points.reserve(nbSides);
    vec2 xy = vec2(-0.0f);
    for (int i = 0; i < nbSides; i++) {
        points.push_back(xy);
        xy += vec2(cos(2.0 * i * pi<double>() / nbSides),
                   sin(2.0 * i * pi<double>() / nbSides));
    }
}

void Polygon::render(void) const {
    glBegin(GL_POLYGON);
    for (const auto& vertex : points) {
        glVertex3fv(value_ptr(vertex));
    }
    glEnd();
}

void displayPolygon(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    polygon.render();
    glFlush();
}