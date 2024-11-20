#ifndef TILING_APP_H
#define TILING_APP_H

#include "edge.h"
#include "polygon.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <list>
#include <memory>
#include <vector>

/// @brief Manages Polygon instances and an OpenGL shader program.
/// Non-copyable.
class TilingApp {
    std::vector<std::shared_ptr<Polygon>> polygons{};
    unsigned shaderProgram{};
    GLFWwindow* window{};
    std::list<Edge> edges{};

    void log(const char* log) const;
    void initGlfwKeyCallback();
    void destroyGL(const bool destroyProgram = true);

  public:
    TilingApp(GLFWwindow* window);
    ~TilingApp();
    TilingApp(const TilingApp&) = delete;
    TilingApp& operator=(const TilingApp&) = delete;
    TilingApp(TilingApp&&);
    TilingApp& operator=(TilingApp&&);
    void addPolygon(int nbSides);
    void addPolygon(int nbSides, unsigned polygonToBindTo);
    void render() const;
    void debug() const;
};

static_assert(!std::is_copy_constructible<TilingApp>::value);
static_assert(!std::is_copy_assignable<TilingApp>::value);

void keyCallback(GLFWwindow* window, int key, int scancode, int action,
                 int mods);

#endif /* TILING_APP_H */