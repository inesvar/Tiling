#include "program.h"
#include "utils.h"
#include <glad/glad.h>

/// @brief Position vec2 `points` on the xy plane using uniform mat3x2
/// `position`.
static const char* minimalVertexShader =
    "#version 330 core\n"
    "layout (location = 0) in vec2 points;\n"
    "// location is the id of the vertex attribute\n"
    "uniform mat3x2 position3x2;\n"
    "uniform mat3x2 view3x2;\n"
    "uniform vec2 windowSize;\n"
    "void main() {\n"
    "    mat3 position = mat3(position3x2);\n"
    "    mat3 view = mat3(view3x2);\n"
    "    vec3 pos = view * position * vec3(points, 1.0);\n"
    "    gl_Position = vec4(pos.xy * windowSize, 0.0, 1.0);\n"
    "}\n";

/// @brief Color fragment using uniform vec3 `color`.
static const char* minimalFragmentShader =
    "#version 330 core\n"
    "uniform vec3 color;\n"
    "void main() {\n"
    "    gl_FragColor = vec4(color, 1.0);\n"
    "}\n";

bool createMinimalProgram(unsigned& program) {
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
