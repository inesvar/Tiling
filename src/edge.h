#ifndef EDGE_H
#define EDGE_H

#include "polygon.h"

struct Edge {
    std::shared_ptr<Polygon> polygon{};
    int edge{};

    Edge(std::shared_ptr<Polygon> polygon, int edge)
        : polygon(polygon), edge(edge) {};
    void render(const unsigned shaderProgram) const;
};

#endif /* EDGE_H */