#include "config.h"
#include "polygon.h"
#include "utils.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>
#include <memory>
#include <string>

const int WINDOW_WIDTH = 600;
const int WINDOW_HEIGHT = 600;

const char* minimalVertexShader =
    "#version 330 core\n"
    "layout (location = 0) in vec2 points;\n"
    "// location is the id of the vertex attribute\n"
    "uniform mat3x2 position;\n"
    "void main() {\n"
    "    vec2 pos = position * vec3(points, 1.0);\n"
    "    gl_Position = vec4(pos, 0.0, 1.0);\n"
    "}\n";

const char* minimalFragmentShader = "#version 330 core\n"
                                    "uniform vec3 color;\n"
                                    "void main() {\n"
                                    "    gl_FragColor = vec4(color, 1.0);\n"
                                    "}\n";

bool createProgram(unsigned& program);
std::string getWindowTitle();
void framebufferSizeCallback(GLFWwindow* window, int height, int width);
void keyCallback(GLFWwindow* window, int key, int scancode, int action,
                 int mods);

int main() {
    if (!glfwInit()) {
        logError("Failed to initialize GLFW");
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT,
                                          getWindowTitle().data(), NULL, NULL);

    if (!window) {
        logError("Failed to create window");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        logError("Failed to load GLAD");
        return -1;
    }
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSwapInterval(1);

    unsigned program;
    if (!createProgram(program)) {
        return -1;
    }
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glUseProgram(program);

    std::vector<std::shared_ptr<Polygon>> polygons{};
    glfwSetWindowUserPointer(window, &polygons);
    polygons.reserve(6);
    polygons.emplace_back(new Polygon(3));
    for (int i = 0; i < 4; i++) {
        polygons.emplace_back(new Polygon(3 + i % 2));
        polygons.back()->bindTo(polygons[i]);
    }
    for (auto& polygon : polygons) {
        polygon->debug();
    }

    glfwSetKeyCallback(window, keyCallback);

    while (!glfwWindowShouldClose(window)) {
        double time = glfwGetTime();
        glClear(GL_COLOR_BUFFER_BIT);
        /* for (auto& polygon : polygons) {
            polygon->positionAt(polygon->getFirstVertex(),
                                rotate(0.031415, polygon->getFirstEdge()) +
                                    polygon->getFirstVertex());
        } */
        for (auto& polygon : polygons) {
            polygon->render(program);
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(program);
    // delete vertex buffers and vertex arrays
    // BEFORE GLFW is terminated
    polygons.clear();
    glfwDestroyWindow(window);
    glfwTerminate();
}

void framebufferSizeCallback(__attribute__((unused)) GLFWwindow* window,
                             int width, int height) {
    // NOTE: this probably only works with one window...
    glViewport(0, 0, width, height);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action,
                 int mods) {
    static std::array<bool, 6> pressedPreviously{};

    void* ptr = glfwGetWindowUserPointer(window);
    auto* polygons = static_cast<std::vector<std::shared_ptr<Polygon>>*>(ptr);

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    } else if (key == GLFW_KEY_3) {
        if (action == GLFW_PRESS && !pressedPreviously[0])
            polygons->emplace_back(new Polygon(3));
        pressedPreviously[0] = action;
    } else if (key == GLFW_KEY_4) {
        if (action == GLFW_PRESS && !pressedPreviously[1])
            polygons->emplace_back(new Polygon(4));
        pressedPreviously[0] = action;
    } else if (key == GLFW_KEY_5) {
        if (action == GLFW_PRESS && !pressedPreviously[2])
            polygons->emplace_back(new Polygon(5));
        pressedPreviously[0] = action;
    } else if (key == GLFW_KEY_6) {
        if (action == GLFW_PRESS && !pressedPreviously[3])
            polygons->emplace_back(new Polygon(6));
        pressedPreviously[0] = action;
    } else if (key == GLFW_KEY_7) {
        if (action == GLFW_PRESS && !pressedPreviously[4])
            polygons->emplace_back(new Polygon(7));
        pressedPreviously[0] = action;
    } else if (key == GLFW_KEY_8) {
        if (action == GLFW_PRESS && !pressedPreviously[5])
            polygons->emplace_back(new Polygon(8));
        pressedPreviously[0] = action;
    }
}

std::string getWindowTitle() {
    return "Tiling V" + std::to_string(VERSION_MAJOR) + "." +
           std::to_string(VERSION_MINOR);
}

bool createProgram(unsigned& program) {
    // create vertex shader
    unsigned vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &minimalVertexShader, NULL);
    glCompileShader(vertexShader);

    int success;
    char text[512];
    char* infoLog = text;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        logError(infoLog);
        return false;
    }

    // create fragment shader
    unsigned fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &minimalFragmentShader, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        logError(infoLog);
        return false;
    }

    // link shaders in a program
    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        logError(infoLog);
        return false;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return true;
}