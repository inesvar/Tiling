#ifndef POLYGON_H
#define POLYGON_H

#include <glm/vec2.hpp>
#include <vector>

class Polygon {
    std::vector<glm::vec2> points{};

  public:
    Polygon(int nbSides);
    void render(void) const;
};

void displayPolygon(void);

#endif /* POLYGON_H */