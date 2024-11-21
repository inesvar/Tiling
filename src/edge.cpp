#include "edge.h"
#include <glm/glm.hpp>
#include <iostream>

void Edge::render(const unsigned shaderProgram) const {
    polygon->highlightEdge(shaderProgram, edge);
}

bool Edge::connectedTo(const Edge& other) const {
    bool connected = false;
    glm::vec2 other_a = other.polygon->getVertex(other.edge);
    glm::vec2 other_b = other.polygon->getVertex(other.edge + 1);
    glm::vec2 this_a = polygon->getVertex(edge);
    glm::vec2 this_b = polygon->getVertex(edge + 1);
    std::cout << other_a.x << ", " << other_a.y << std::endl;
    std::cout << this_a.x << ", " << this_a.y << std::endl;
    std::cout << other_b.x << ", " << other_b.y << std::endl;
    std::cout << this_b.x << ", " << this_b.y << std::endl;
    float distance1 = glm::distance(other_a, this_b);
    float distance2 = glm::distance(other_b, this_a);
    return (distance1 < 1e-7) && (distance2 < 1e-7);
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