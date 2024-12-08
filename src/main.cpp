#include "config.h"
#include "tilingApp.h"
#include "utils.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>
#include <memory>
#include <string>

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

    GLFWwindow* window =
        glfwCreateWindow(DEFAULT_WINDOW_SIZE, DEFAULT_WINDOW_SIZE,
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
    glViewport(0, 0, DEFAULT_WINDOW_SIZE, DEFAULT_WINDOW_SIZE);
    glfwSwapInterval(1);
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (glfwRawMouseMotionSupported()) {
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    } else {
        logError("Raw mouse input not supported");
    }

    {
        std::unique_ptr<TilingApp> app(new TilingApp(window));
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