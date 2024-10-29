#include "polygon.h"
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

void displayPolygon(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POLYGON);
    glm::vec3 thing = glm::vec3(0.0f);
    glVertex3fv(glm::value_ptr(thing));
    glVertex3f(0.5, 0.0, 0.0);
    glVertex3f(0.5, 0.5, 0.0);
    glVertex3f(0.25, 0.625, 0.0);
    glVertex3f(0.0, 0.5, 0.0);
    glEnd();
    glFlush();
}