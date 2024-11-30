#include "tilingApp.h"
#include "edge.h"
#include "program.h"
#include "utils.h"
#include <algorithm>
#include <cassert>
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat3x2.hpp>
#include <iostream>

TilingApp::TilingApp(GLFWwindow* window) : window(window) {
    assert(createMinimalProgram(shaderProgram));
    initGlfwCallbacks();
    edges.emplace_back(std::make_shared<Polygon>(2), 0);
    currentEdge = edges.begin();
    updateViewCenter();
    viewMatrix = glm::mat3x2(1.0);
    log(" was " GREEN "created" RESET ".");
}

TilingApp::~TilingApp() {
    destroyGL();
    glfwSetWindowUserPointer(window, nullptr);
    log(" was " RED "deleted" RESET ".");
}

/// @brief Create a Polygon with `nbSides` on the position of `currentEdge`.
/// The new polygon sides are stored in `edges` or `links`.
///
/// First all new edges are stored in `edges`. Then overlapping edges
/// contiguous in `edges` are detected and moved to `links`. Non-contiguous
/// overlapping edges aren't detected and remain stored in `edges`.
/// @param nbSides
void TilingApp::addPolygon(int nbSides) {
    if (polygons.empty()) {
        polygons.emplace_back(new Polygon(nbSides));
        edges.clear();
        for (int i = 0; i < nbSides; i++) {
            edges.emplace_back(polygons.back(), i);
        }
        currentEdge = edges.begin();
        updateViewCenter();
    } else {
        // create new polygon
        polygons.emplace_back(new Polygon(nbSides));
        polygons.back()->bindTo(currentEdge->polygon, currentEdge->edge);
        // create a list of this polygon edges
        auto newEdges = std::list<Edge>{};
        for (int i = 0; i < nbSides; i++) {
            newEdges.emplace_back(polygons.back(), i);
        }
        // store the new edges in `edges` (between iterators `left` and `right`)
        auto right = circularNext(currentEdge);
        std::list<Edge>::const_iterator left =
            edges.insert(right, newEdges.begin(), newEdges.end());
        // for any consecutive overlapping edges
        // (1) update `links` (insert the two overlapping edges)
        // (2) update `edges` (remove the two overlapping edges)
        // /!\ pay attention not to invalidate `left`
        while (right->connectedTo(*circularPrev(right))) {
            // cf (1)
            links.emplace(*right, *circularPrev(right));
            links.emplace(*circularPrev(right), *right);
            // cf /!\ .
            if (circularPrev(right) == left) {
                left = circularNext(right);
                // cf (2)
                edges.erase(circularPrev(right));
                right = circularNext(right);
                edges.erase(circularPrev(right));
                break;
            }
            // cf (2)
            edges.erase(circularPrev(right));
            right = circularNext(right);
            edges.erase(circularPrev(right));
        }
        while (left->connectedTo(*circularPrev(left))) {
            // cf (1)
            links.emplace(*left, *circularPrev(left));
            links.emplace(*circularPrev(left), *left);
            // cf (2)
            edges.erase(circularPrev(left));
            left = circularNext(left);
            edges.erase(circularPrev(left));
        }
        currentEdge = left;
        updateViewCenter();
    }
}

/// @brief Remove the Polygon created last. Variables `edges` and `links` are
/// updated accordingly.
///
/// First, edges overlapping with the most recent polygon are added to `edges`
/// and removed from `links`. Then the sides from the last polygon are
/// removed from `edges` and the polygon is removed from `polygons`.
///
/// @note This implementation supposes that the edges of the last polygon are
/// all adjacent in `edges` (there sequential in the _circular_ list, but may
/// not be sequential in the underlying list).
///
/// @note Removing the polygon pointed by `currentEdge` instead of the last
/// polygon might result in polygons not being linked anymore.
void TilingApp::removeLastPolygon() {
    if (polygons.empty()) {
        return;
    } else if (polygons.size() == 1) {
        removeAllPolygons();
        return;
    }
    auto lastPolygon = polygons.back();
    std::clog << lastPolygon.get() << std::endl;
    // TODO using a const iterator is not needed so no need to use it
    std::list<Edge>::const_iterator left =
        std::find_if(edges.begin(), edges.end(), [lastPolygon](Edge& edge) {
            return (edge.polygon == lastPolygon);
        });
    std::list<Edge>::const_reverse_iterator _right =
        std::find_if(edges.rbegin(), edges.rend(), [lastPolygon](Edge& edge) {
            return (edge.polygon == lastPolygon);
        });
    if (left == edges.end() || _right == edges.rend()) {
        // FIXME
        logError("Removal of a polygon with no accessible sides is not "
                 "implemented yet.");
        return;
    }
    std::clog << "Left:" << left->polygon.get() << " " << left->edge
              << std::endl;
    std::clog << "_right:" << _right->polygon.get() << " " << _right->edge
              << std::endl;
    auto right = std::prev(_right.base());
    std::clog << "Right:" << right->polygon.get() << " " << right->edge
              << std::endl;
    if (left->edge > right->edge) {
        std::clog << "Circular list trouble." << std::endl;
        // Unfortunately lastPolygon edges aren't sequential as they span the
        // end and begining of the list.
        auto futureRight =
            std::find_if(edges.begin(), edges.end(), [lastPolygon](Edge& edge) {
                return (edge.polygon != lastPolygon);
            });
        edges.splice(edges.end(), edges, edges.begin(), futureRight);
        removeLastPolygon();
        return;
    }
    debug();
    int rightEdge = right->edge;
    right = std::next(right);
    for (int side = rightEdge + 1; side <= lastPolygon->nbSides - 1; side++) {
        auto edge = Edge{lastPolygon, side};
        auto linkedEdge = links.at(edge);
        right = edges.insert(right, linkedEdge);
        links.erase(edge);
        links.erase(linkedEdge);
    }
    debug();
    for (int side = left->edge - 1; side >= 0; side--) {
        auto edge = Edge{lastPolygon, side};
        auto linkedEdge = links.at(edge);
        edges.insert(left, linkedEdge);
        links.erase(edge);
        links.erase(linkedEdge);
    }
    if (currentEdge->polygon == lastPolygon) {
        currentEdge = circularPrev(left);
        updateViewCenter();
    }
    debug();
    std::clog << "currentEdge" << currentEdge->polygon.get() << " "
              << currentEdge->edge << std::endl;
    edges.erase(left, right);
    debug();
    polygons.pop_back();
}

void TilingApp::debug() const {
    /* for (auto& polygon : polygons) {
        polygon->debug();
    } */
    for (auto& edge : edges) {
        std::clog << edge.polygon.get() << " " << edge.edge << std::endl;
    }
    std::clog << std::endl;
}
void TilingApp::render() const {

    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    int positionUniform = glGetUniformLocation(shaderProgram, "view3x2");
    glUniformMatrix3x2fv(positionUniform, 1, GL_FALSE, value_ptr(viewMatrix));

    for (auto& polygon : polygons) {
        polygon->render(shaderProgram);
    }
    for (auto& edge : edges) {
        edge.underline(shaderProgram);
    }
    currentEdge->highlight(shaderProgram);

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

void TilingApp::initGlfwCallbacks() {
    glfwSetWindowUserPointer(window, this);
    glfwSetKeyCallback(window, TilingApp::keyCallback);
    glfwSetScrollCallback(window, TilingApp::scrollCallback);
}

void TilingApp::removeAllPolygons() {
    edges.clear();
    polygons.clear();
    edges.emplace_back(std::make_shared<Polygon>(2), 0);
    links.clear();
    currentEdge = edges.begin();
    updateViewCenter();
}

std::list<Edge>::const_iterator
TilingApp::circularNext(std::list<Edge>::const_iterator& edge) const {
    auto incremented = std::next(edge);
    if (incremented == edges.cend()) {
        incremented = edges.cbegin();
    }
    return incremented;
}

std::list<Edge>::const_iterator
TilingApp::circularPrev(std::list<Edge>::const_iterator& edge) const {
    auto toDecrement = edge;
    if (toDecrement == edges.cbegin()) {
        toDecrement = edges.cend();
    }
    return std::prev(toDecrement);
}

void TilingApp::zoomIn() { viewMatrix *= 2.0; }

void TilingApp::zoomOut() { viewMatrix /= 2.0; }

void TilingApp::translate(const glm::vec2& direction) {
    viewMatrix[2] -= direction * viewMatrix[0].x;
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
        case GLFW_KEY_9:
        case GLFW_KEY_KP_9:
            addPolygon(9);
            break;
        case GLFW_KEY_0:
        case GLFW_KEY_KP_0:
            addPolygon(10);
            break;
        case GLFW_KEY_TAB: {
            if (mods && GLFW_MOD_SHIFT) {
                currentEdge = circularPrev(currentEdge);
            } else {
                currentEdge = circularNext(currentEdge);
            }
            updateViewCenter();
            break;
        }
        case GLFW_KEY_DELETE:
            removeAllPolygons();
            break;
        case GLFW_KEY_BACKSPACE:
            removeLastPolygon();
            break;
        case GLFW_KEY_KP_ADD:
            zoomIn();
            break;
        case GLFW_KEY_KP_SUBTRACT:
            zoomOut();
            break;
    }
}

void TilingApp::handleScroll(const double xoffset, const double yoffset) {
    translate(0.05f * glm::vec2(-xoffset, yoffset));
}

void TilingApp::keyCallback(GLFWwindow* window, int key,
                            __attribute__((unused)) int scancode,
                            __attribute__((unused)) int action, int mods) {

    void* ptr = glfwGetWindowUserPointer(window);
    auto* app = static_cast<TilingApp*>(ptr);

    if (action == GLFW_PRESS) {
        app->handleKeyPress(key, mods);
    }
}

void TilingApp::scrollCallback(GLFWwindow* window, double xoffset,
                               double yoffset) {
    void* ptr = glfwGetWindowUserPointer(window);
    auto* app = static_cast<TilingApp*>(ptr);

    app->handleScroll(xoffset, yoffset);
}

void TilingApp::updateViewCenter() {
    viewMatrix[2] = -viewMatrix[0].x * currentEdge->getFirstVertex();
}