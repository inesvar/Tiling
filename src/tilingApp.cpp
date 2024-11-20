#include "tilingApp.h"
#include "edge.h"
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
      window(std::move(other.window)), edges(std::move(other.edges)),
      currentEdge(std::move(currentEdge)) {
    other.shaderProgram = 0;
    initGlfwKeyCallback();
    log(" was " BLUE "created using move" RESET ".");
}

TilingApp& TilingApp::operator=(TilingApp&& other) {
    if (&other != this) {
        polygons = std::move(other.polygons);
        destroyGL((shaderProgram == other.shaderProgram));
        shaderProgram = other.shaderProgram;
        other.shaderProgram = 0;
        window = std::move(other.window);
        edges = std::move(other.edges);
        currentEdge = std::move(other.currentEdge);
    }
    log(" was " BLUE "assigned using move" RESET ".");
    return *this;
}

void TilingApp::addPolygon(int nbSides) {
    if (polygons.empty()) {
        polygons.emplace_back(new Polygon(nbSides));
        for (int i = 0; i < nbSides; i++) {
            edges.emplace_back(polygons.back(), i);
        }
        currentEdge = edges.begin();
    } else {
        polygons.emplace_back(new Polygon(nbSides));
        polygons.back()->bindTo(currentEdge->polygon, currentEdge->edge + 1);
        // TODO remove duplicated edges, insert in the right place
        for (int i = 0; i < nbSides; i++) {
            edges.emplace_back(polygons.back(), i);
        }
    }
}

void TilingApp::addPolygonNextToLast(int nbSides) {
    auto polygon = std::make_shared<Polygon>(nbSides);
    polygon->bindTo(polygons.back());
    polygons.push_back(polygon);
    for (int i = 0; i < nbSides; i++) {
        edges.emplace_back(polygons.back(), i);
    }
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
    currentEdge->render(shaderProgram);

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
}

void TilingApp::initGlfwKeyCallback() {
    glfwSetWindowUserPointer(window, this);
    glfwSetKeyCallback(window, TilingApp::keyCallback);
}

void TilingApp::keyCallback(GLFWwindow* window, int key, int scancode,
                            int action, int mods) {

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
        case GLFW_KEY_TAB: {
            if (mods && GLFW_MOD_SHIFT) {
                if (app->currentEdge == app->edges.begin()) {
                    app->currentEdge = app->edges.end();
                }
                app->currentEdge--;
            } else {
                if (++app->currentEdge == app->edges.end()) {
                    app->currentEdge = app->edges.begin();
                }
            }
        }
    }
}