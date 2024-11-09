#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

const int WIDTH = 500;
const int HEIGHT = 500;

const char* minimalVertexShader =
    "#version 330 core\n"
    "layout (location = 0) in vec2 pos;"
    "// location is the id of the vertex attribute\n"
    "void main() {\n"
    "    gl_Position = vec4(pos, 0.0, 1.0);\n"
    "}\n";

const char* minimalFragmentShader = "#version 330 core\n"
                                    "uniform vec4 color;\n"
                                    "void main() {\n"
                                    "    gl_FragColor = color;\n"
                                    "}\n";

void createVboAndVao(unsigned& vbo, unsigned& vao);
bool createProgram(unsigned& program);
glm::vec4 purpleGradient();

int main() {
    if (!glfwInit()) {
        return -1;
    }
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Minimal", NULL, NULL);
    if (!window) {
        return -1;
    }

    glfwMakeContextCurrent(window);
    gladLoadGL();

    glViewport(0, 0, WIDTH, HEIGHT);

    unsigned triangleData, triangleArray, displayInColor;
    createVboAndVao(triangleData, triangleArray);
    if (!createProgram(displayInColor)) {
        return -1;
    }

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        int colorUniform = glGetUniformLocation(displayInColor, "color");
        glUniform4fv(colorUniform, 1, glm::value_ptr(purpleGradient()));
        glUseProgram(displayInColor);
        glBindVertexArray(triangleArray);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(displayInColor);
    glDeleteVertexArrays(1, &triangleData);
    glDeleteBuffers(1, &triangleData);

    glfwDestroyWindow(window);
    glfwTerminate();
}

void createVboAndVao(unsigned& vbo, unsigned& vao) {
    // create VBO (a memory block on GPU)
    glGenBuffers(1, &vbo);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    float triangle[3][2] = {{-0.5, -0.5}, {0.5, -0.5}, {0.0, 0.3}};
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);

    // set vertex attributes (rules to read the VBO currently bound to
    // GL_ARRAY_BUFFER)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    // vertex attribute 0 is now linked to the VBO containing triangle
    glEnableVertexAttribArray(0);
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
        std::cout << infoLog << std::endl;
        return false;
    }

    // create fragment shader
    unsigned fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &minimalFragmentShader, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << infoLog << std::endl;
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
        std::cout << infoLog << std::endl;
        return false;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return true;
}

glm::vec4 purpleGradient() {
    double time = glfwGetTime();
    glm::vec4 color = glm::vec4(1.0, cos(time) * 0.5 + 0.5, 1.0, 1.0);
    return color;
}