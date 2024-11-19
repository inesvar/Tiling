#include "program.h"
#include "utils.h"
#include <glad/glad.h>

static const char* minimalVertexShader =
    "#version 330 core\n"
    "layout (location = 0) in vec2 points;\n"
    "// location is the id of the vertex attribute\n"
    "uniform mat3x2 position;\n"
    "void main() {\n"
    "    vec2 pos = position * vec3(points, 1.0);\n"
    "    gl_Position = vec4(pos, 0.0, 1.0);\n"
    "}\n";

static const char* minimalFragmentShader =
    "#version 330 core\n"
    "uniform vec3 color;\n"
    "void main() {\n"
    "    gl_FragColor = vec4(color, 1.0);\n"
    "}\n";

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
