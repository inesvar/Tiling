#include "config.h"
#include "polygon.h"
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

const int WINDOW_WIDTH = 600;
const int WINDOW_HEIGHT = 600;

std::string getWindowTitle();
void framebufferSizeCallback(GLFWwindow* window, int height, int width);
void processInput(GLFWwindow* window);

int main() {
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT,
                                          getWindowTitle().data(), NULL, NULL);

    if (!window) {
        std::cout << "Failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to load GLAD" << std::endl;
        return -1;
    }
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        processInput(window);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}

void framebufferSizeCallback(__attribute__((unused)) GLFWwindow* window,
                             int height, int width) {
    // NOTE: this probably only works with one window...
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

std::string getWindowTitle() {
    return "Tiling V" + std::to_string(VERSION_MAJOR) + "." +
           std::to_string(VERSION_MINOR);
}