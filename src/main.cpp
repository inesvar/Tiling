#include <GL/glut.h>
#include <iostream>
#include "config.h"

void displayPolygon(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POLYGON);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(0.5, 0.0, 0.0);
        glVertex3f(0.5, 0.5, 0.0);
        glVertex3f(0.25, 0.625, 0.0);
        glVertex3f(0.0, 0.5, 0.0);
    glEnd();
    glFlush();
}

int main(int argc, char** argv)
{
    std::cout << "Version " << VERSION_MAJOR << "." << VERSION_MINOR << std::endl;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(1300, 100);
    glutCreateWindow("Hello world !");
    glutDisplayFunc(displayPolygon);
    glutMainLoop();
}