#ifndef TILING_APP_H
#define TILING_APP_H

#include "edge.h"
#include "polygon.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <list>
#include <memory>
#include <unordered_map>
#include <vector>

// TODO update with the implementation

/// @brief Manages Polygon instances, an edge cursor and an OpenGL shader
/// program. Non-copyable.
///
/// @arg `currentEdge` Edge cursor that you can move from edge to edge to
/// choose where to add a new polygon.
/// This cursor can only access polygon sides stored in `edges`.
///
/// @arg `edges` Polygon sides accessible by the edge cursor `currentEdge`.
/// Linked list with bidirectional circular traversal using
/// `TilingApp::circularNext` and `TilingApp::circularPrev`.
///
/// @arg `links` Polygon sides unreachable by the edge cursor `currentEdge`.
/// Hashmap mapping edge to edge.
///
/// @note
/// In principle, overlapping edges of connected polygons should be stored in
/// `links` while other "unsaturated" edges should be stored in `edges`. In
/// practice, the detection of overlapping edges when instantiating a new
/// polygon is not perfect. There can remain undetected overlapping polygon
/// sides in `edges`. However, the current implementation has the useful feature
/// that adjacent elements in `edges` are always adjacent sides, and that
/// polygons are always all connected.
class TilingApp {
    std::vector<std::shared_ptr<Polygon>> polygons{};
    unsigned shaderProgram{};
    GLFWwindow* window{};
    std::list<Edge> edges{};
    std::unordered_map<Edge, Edge, EdgeHash> links{};
    std::list<Edge>::const_iterator currentEdge{};

    void log(const char* log) const;
    void initGlfwKeyCallback();
    void destroyGL(const bool destroyProgram = true);
    void handleKeyPress(const int key, const int mods);
    std::list<Edge>::const_iterator
    circularNext(std::list<Edge>::const_iterator& edge) const;
    std::list<Edge>::const_iterator
    circularPrev(std::list<Edge>::const_iterator& edge) const;

  public:
    TilingApp(GLFWwindow* window);
    ~TilingApp();
    TilingApp(const TilingApp&) = delete;
    TilingApp& operator=(const TilingApp&) = delete;
    /* TilingApp(TilingApp&&);
    TilingApp& operator=(TilingApp&&); */
    void addPolygon(int nbSides);
    void removeAllPolygons();
    void render() const;
    void debug() const;

    static void keyCallback(GLFWwindow* window, int key, int scancode,
                            int action, int mods);
};

static_assert(!std::is_copy_constructible<TilingApp>::value);
static_assert(!std::is_copy_assignable<TilingApp>::value);

#endif /* TILING_APP_H */