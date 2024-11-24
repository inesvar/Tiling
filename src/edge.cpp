#include "edge.h"
#include <glm/glm.hpp>
#include <iostream>

void Edge::highlight(const unsigned shaderProgram) const {
    polygon->highlightEdge(shaderProgram, edge);
}

void Edge::underline(const unsigned shaderProgram) const {
    polygon->underlineEdge(shaderProgram, edge);
}

bool Edge::connectedTo(const Edge& other) const {
    glm::vec2 other_a = other.polygon->getVertex(other.edge);
    glm::vec2 other_b = other.polygon->getVertex(other.edge + 1);
    glm::vec2 this_a = polygon->getVertex(edge);
    glm::vec2 this_b = polygon->getVertex(edge + 1);
    float distance1 = glm::distance(other_a, this_b);
    float distance2 = glm::distance(other_b, this_a);
    std::clog << distance1 << ", " << distance2 << std::endl;
    return (distance1 < 1e-5) && (distance2 < 1e-5);
};

std::size_t EdgeHash::operator()(const Edge& e) const {
    std::string text =
        std::to_string(reinterpret_cast<uintptr_t>(e.polygon.get())) +
        std::to_string(e.edge);
    return std::hash<std::string>()(text);
}

bool operator==(const Edge& lhs, const Edge& rhs) {
    return lhs.polygon == rhs.polygon && lhs.edge == rhs.edge;
}