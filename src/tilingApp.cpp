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
    edges.emplace_back(std::make_shared<Polygon>(2), 0);
    currentEdge = edges.begin();
    log(" was " GREEN "created" RESET ".");
}

TilingApp::~TilingApp() {
    destroyGL();
    glfwSetWindowUserPointer(window, nullptr);
    log(" was " RED "deleted" RESET ".");
}

/* TilingApp::TilingApp(TilingApp&& other)
    : polygons(std::move(other.polygons)), shaderProgram(other.shaderProgram),
      window(std::move(other.window)), edges(std::move(other.edges)),
      currentEdge(std::move(other.currentEdge)) {
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
        initGlfwKeyCallback();
    }
    log(" was " BLUE "assigned using move" RESET ".");
    return *this;
} */

void TilingApp::addPolygon(int nbSides) {
    if (polygons.empty()) {
        polygons.emplace_back(new Polygon(nbSides));
        edges.clear();
        for (int i = 0; i < nbSides; i++) {
            edges.emplace_back(polygons.back(), i);
        }
        currentEdge = edges.begin();
    } else {
        polygons.emplace_back(new Polygon(nbSides));
        polygons.back()->bindTo(currentEdge->polygon, currentEdge->edge);
        auto oldEdge = currentEdge;
        auto newEdges = std::list<Edge>{};
        for (int i = 1; i < nbSides; i++) {
            newEdges.emplace_back(polygons.back(), i);
        }
        currentEdge =
            edges.insert(currentEdge, newEdges.begin(), newEdges.end());
        edges.erase(oldEdge);
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

void TilingApp::removeAllPolygons() {
    edges.clear();
    polygons.clear();
    edges.emplace_back(std::make_shared<Polygon>(2), 0);
    links.clear();
    currentEdge = edges.begin();
}

void TilingApp::handleKeyPress(const int key, const int mods) {
    switch (key) {
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, true);
            break;
        case GLFW_KEY_3:
        case GLFW_KEY_KP_3:
            addPolygon(3);
            break;
        case GLFW_KEY_4:
        case GLFW_KEY_KP_4:
            addPolygon(4);
            break;
        case GLFW_KEY_5:
        case GLFW_KEY_KP_5:
            addPolygon(5);
            break;
        case GLFW_KEY_6:
        case GLFW_KEY_KP_6:
            addPolygon(6);
            break;
        case GLFW_KEY_7:
        case GLFW_KEY_KP_7:
            addPolygon(7);
            break;
        case GLFW_KEY_8:
        case GLFW_KEY_KP_8:
            addPolygon(8);
            break;
        case GLFW_KEY_TAB: {
            if (mods && GLFW_MOD_SHIFT) {
                if (currentEdge == edges.begin()) {
                    currentEdge = edges.end();
                }
                currentEdge--;
            } else {
                if (++currentEdge == edges.end()) {
                    currentEdge = edges.begin();
                }
            }
            break;
        }
        case GLFW_KEY_DELETE:
            removeAllPolygons();
            break;
    }
}

void TilingApp::keyCallback(GLFWwindow* window, int key, int scancode,
                            int action, int mods) {

    void* ptr = glfwGetWindowUserPointer(window);
    auto* app = static_cast<TilingApp*>(ptr);

    if (action == GLFW_PRESS) {
        app->handleKeyPress(key, mods);
    }
}