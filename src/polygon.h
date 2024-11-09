#ifndef POLYGON_H
#define POLYGON_H

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>

class Polygon {
    std::vector<glm::vec2> points{};
    glm::vec2 center{};
    glm::vec3 color{};
    unsigned vbo{};
    unsigned vao{};
    void init(int nbSides);
    void initGL();

  public:
    Polygon(int nbSides);
    Polygon(const glm::vec2& a, const glm::vec2& b, int nbSides);
    ~Polygon();
    void cleanGL();
    void render(unsigned program) const;
};

#endif /* POLYGON_H */