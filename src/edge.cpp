#include "edge.h"

void Edge::render(const unsigned shaderProgram) const {
    polygon->highlightEdge(shaderProgram, edge);
}