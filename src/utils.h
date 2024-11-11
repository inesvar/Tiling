#ifndef UTILS_H
#define UTILS_H

#include <glm/glm.hpp>

#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define BLUE "\033[1;34m"
#define RESET "\033[0m"

glm::vec2 rotate(const glm::vec2& vector, float radianAngle);
glm::vec3 nextColor(float hueStep = 30);
void logError(const char* error);

#endif /* UTILS_H */