#ifndef POLYGON_H
#define POLYGON_H

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>

class Polygon {
    std::vector<glm::vec2> points{};
    glm::vec3 color;

  public:
    Polygon(int nbSides);
    Polygon(const glm::vec2& a, const glm::vec2& b, int nbSides);
    void render(void) const;
};

void displayPolygon(void);

#endif /* POLYGON_H */