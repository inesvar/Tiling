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
    static std::array<bool, 6> pressedPreviously{};

    void* ptr = glfwGetWindowUserPointer(window);
    auto* app = static_cast<TilingApp*>(ptr);

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    } else if (key == GLFW_KEY_3) {
        if (action == GLFW_PRESS && !pressedPreviously[0])
            app->addPolygon(3);
        pressedPreviously[0] = action;
    } else if (key == GLFW_KEY_4) {
        if (action == GLFW_PRESS && !pressedPreviously[1])
            app->addPolygon(4);
        pressedPreviously[0] = action;
    } else if (key == GLFW_KEY_5) {
        if (action == GLFW_PRESS && !pressedPreviously[2])
            app->addPolygon(5);
        pressedPreviously[0] = action;
    } else if (key == GLFW_KEY_6) {
        if (action == GLFW_PRESS && !pressedPreviously[3])
            app->addPolygon(6);
        pressedPreviously[0] = action;
    } else if (key == GLFW_KEY_7) {
        if (action == GLFW_PRESS && !pressedPreviously[4])
            app->addPolygon(7);
        pressedPreviously[0] = action;
    } else if (key == GLFW_KEY_8) {
        if (action == GLFW_PRESS && !pressedPreviously[5])
            app->addPolygon(8);
        pressedPreviously[0] = action;
    }
}