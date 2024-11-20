#include "tilingApp.h"
#include "program.h"
#include "utils.h"
#include <cassert>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>

TilingApp::TilingApp(GLFWwindow* window) : window(window) {
    assert(createMinimalProgram(shaderProgram));
    initGlfwKeyCallback();
    log(" was " GREEN "created" RESET ".");
}

TilingApp::~TilingApp() {
    destroyGL();
    glfwSetWindowUserPointer(window, nullptr);
    log(" was " RED "deleted" RESET ".");
}

TilingApp::TilingApp(TilingApp&& other)
    : polygons(std::move(other.polygons)), shaderProgram(other.shaderProgram),
      window(std::move(other.window)) {
    other.shaderProgram = 0;
    initGlfwKeyCallback();
    log(" was " BLUE "created using move" RESET ".");
}

TilingApp& TilingApp::operator=(TilingApp&& other) {
    if (&other != this) {
        polygons = std::move(other.polygons);
        shaderProgram = other.shaderProgram;
        window = std::move(other.window);
        other.shaderProgram = 0;
    }
    log(" was " BLUE "assigned using move" RESET ".");
    return *this;
}

void TilingApp::addPolygon(int nbSides) {
    polygons.emplace_back(new Polygon(nbSides));
}

void TilingApp::addPolygon(int nbSides, unsigned polygonToBindTo) {
    polygons.emplace_back(new Polygon(nbSides));
    polygons.back()->bindTo(polygons[polygonToBindTo]);
}

void TilingApp::debug() const {
    for (auto& polygon : polygons) {
        polygon->debug();
    }
}
void TilingApp::render() const {

    // double time = glfwGetTime();
    glClear(GL_COLOR_BUFFER_BIT);
    /* for (auto& polygon : polygons) {
        polygon->positionAt(polygon->getFirstVertex(),
                            rotate(0.031415, polygon->getFirstEdge()) +
                                polygon->getFirstVertex());
    } */

    glUseProgram(shaderProgram);
    for (auto& polygon : polygons) {
        polygon->render(shaderProgram);
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
}

void TilingApp::log(const char* log) const {
    std::clog << "TilingApp" << log << std::endl;
}

void TilingApp::destroyGL(const bool destroyProgram) {
    if (destroyProgram && (shaderProgram)) {
        glDeleteProgram(shaderProgram);
    }
    polygons.clear();
}

void TilingApp::initGlfwKeyCallback() {
    glfwSetWindowUserPointer(window, this);
    glfwSetKeyCallback(window, keyCallback);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action,
                 int mods) {

    if (action != GLFW_PRESS) {
        return;
    }

    void* ptr = glfwGetWindowUserPointer(window);
    auto* app = static_cast<TilingApp*>(ptr);

    switch (key) {
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, true);
            break;
        case GLFW_KEY_3:
            app->addPolygon(3);
            break;
        case GLFW_KEY_4:
            app->addPolygon(4);
            break;
        case GLFW_KEY_5:
            app->addPolygon(5);
            break;
        case GLFW_KEY_6:
            app->addPolygon(6);
            break;
        case GLFW_KEY_7:
            app->addPolygon(7);
            break;
        case GLFW_KEY_8:
            app->addPolygon(8);
            break;
    }
}