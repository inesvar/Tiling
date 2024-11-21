#include "edge.h"

void Edge::render(const unsigned shaderProgram) const {
    polygon->highlightEdge(shaderProgram, edge);
}

std::size_t EdgeHash::operator()(const Edge& e) const {
    std::string text =
        std::to_string(reinterpret_cast<uintptr_t>(e.polygon.get())) +
        std::to_string(e.edge);
    return std::hash<std::string>()(text);
}

bool operator==(const Edge& lhs, const Edge& rhs) {
    return lhs.polygon == rhs.polygon && lhs.edge == rhs.edge;
}