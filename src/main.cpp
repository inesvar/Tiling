#include "config.h"
#include "polygon.h"
#include <GL/glut.h>
#include <string>

std::string getWindowTitle(void);

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(1300, 100);
    glutCreateWindow(getWindowTitle().data());
    glutDisplayFunc(displayPolygon);
    glutMainLoop();
}

std::string getWindowTitle(void) {
    return "Tiling V" + std::to_string(VERSION_MAJOR) + "." + std::to_string(VERSION_MINOR);
}