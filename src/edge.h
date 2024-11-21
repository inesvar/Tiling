#ifndef EDGE_H
#define EDGE_H

#include "polygon.h"
#include <string>

struct Edge {
    std::shared_ptr<Polygon> polygon{};
    int edge{};

    Edge(std::shared_ptr<Polygon> polygon, int edge)
        : polygon(polygon), edge(edge) {};
    void render(const unsigned shaderProgram) const;
    bool connectedTo(const Edge& edge) const;
};

struct EdgeHash {
    std::size_t operator()(const Edge& e) const;
};

bool operator==(const Edge& lhs, const Edge& rhs);

#endif /* EDGE_H */