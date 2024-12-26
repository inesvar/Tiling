#include "utils.h"
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/color_space.hpp>
#include <iostream>

using namespace glm;

int DEFAULT_WINDOW_SIZE = 800;

/// @brief Rotate `point` by `radianAngle` around `(0, 0)`.
/// @param radianAngle
/// @param point defaults to (0, 1)
vec2 rotate(float radianAngle, const vec2& point) {
    // compute the complex multiplication of
    // `point` and $e^{radianAngle * i}$
    mat2 edge = mat2(point.x, -point.y, point.y, point.x);
    return transpose(edge) * vec2(cos(radianAngle), sin(radianAngle));
}

PolygonColor nextColor() {
    static int color = 0;
    color %= 11;
    return static_cast<PolygonColor>(color++);
}

/// @brief Return cursor color (HSV color circle).
vec3 cursorColor() {
    static vec3 color = vec3(1.0f, 0.2f, 0.2f);
    auto returnColor = color;
    auto hsv = hsvColor(color);
    if (hsv.x + 5.0 > 360) {
        hsv.x = (hsv.x + 5.0 - 360);
    } else {
        hsv.x = hsv.x + 5.0;
    }
    color = rgbColor(hsv);
    return returnColor;
}

/// @brief Print `error` in red to the cerr buffer.
/// @param error
void logError(const char* error) {
    std::cerr << RED << error << RESET << std::endl;
}

vec3 getColor(const PolygonColor& color) {
    switch (color) {
        case red: {
            return vec3(0.9, 0.1, 0.1);
        }
        case orange: {
            return vec3(1.0, 0.5, 0.0);
        }
        case yellow: {
            return vec3(1.0, 0.9, 0.0);
        }
        case green: {
            return vec3(0.1, 0.8, 0.1);
        }
        case lightBlue: {
            return vec3(0.6, 0.8, 1.0);
        }
        case darkBlue: {
            return vec3(0.1, 0.1, 0.9);
        }
        case violet: {
            return vec3(0.6, 0.0, 0.9);
        }
        case pink: {
            return vec3(1.0, 0.3, 0.7);
        }
        case brown: {
            return vec3(0.6, 0.3, 0.2);
        }
        case grey: {
            return vec3(0.6, 0.6, 0.6);
        }
        default: {
            return vec3(1.0, 1.0, 1.0);
        }
    }
}