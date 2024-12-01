#include "config.h"
#include "tilingApp.h"
#include "utils.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>
#include <memory>
#include <string>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;

std::string getWindowTitle();
void framebufferSizeCallback(GLFWwindow* window, int height, int width);

int main() {
    // Initialize GLFW, create a window, load GLAD...
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
    glfwSwapInterval(1);
    glClearColor(1.0, 1.0, 1.0, 1.0);

    {
        std::unique_ptr<TilingApp> app(new TilingApp(window));

        // Testing move assignment
        // std::shared_ptr<TilingApp> app2(new TilingApp(window));
        // std::shared_ptr<TilingApp> app(new TilingApp(window));
        // *app = std::move(*app2);
        // Testing move construction
        // std::shared_ptr<TilingApp> app2(new TilingApp(window));
        // std::shared_ptr<TilingApp> app(new TilingApp(std::move(*app2)));

        app->debug();

        while (!glfwWindowShouldClose(window)) {
            app->render();
        }
        // delete GL objects before GLFW is terminated
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}

std::string getWindowTitle() {
    return "Tiling V" + std::to_string(VERSION_MAJOR) + "." +
           std::to_string(VERSION_MINOR);
}