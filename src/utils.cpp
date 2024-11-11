#include "utils.h"
#include <glm/glm.hpp>
#include <glm/gtx/color_space.hpp>
#include <iostream>

using namespace glm;

/// @brief Rotate `point` by `radianAngle` around `(0, 0)`.
/// @param point
/// @param radianAngle
vec2 rotate(const vec2& point, float radianAngle) {
    // compute the complex multiplication of
    // `point` and $e^{radianAngle * i}$
    mat2 edge = mat2(point.x, -point.y, point.y, point.x);
    return transpose(edge) * vec2(cos(radianAngle), sin(radianAngle));
}

/// @brief Return color and add `hueStep` in HSV color circle.
/// @param hueStep defaults to 30.0
vec3 nextColor(float hueStep) {
    static vec3 color = vec3(1.0f, 0.2f, 0.2f);
    auto returnColor = color;
    auto hsv = hsvColor(color);
    if (hsv.x + hueStep > 360) {
        hsv.x = (hsv.x + hueStep - 360);
    } else {
        hsv.x = hsv.x + hueStep;
    }
    color = rgbColor(hsv);
    return returnColor;
}

/// @brief Print `error` in red to the cerr buffer.
/// @param error 
void logError(const char* error) {
    std::cerr << RED << error << RESET << std::endl;
}