#include "color.h"
#include <glm/gtx/color_space.hpp>
#include <glm/vec3.hpp>

glm::vec3 nextColor() {
    static glm::vec3 color = glm::vec3(1.0f, 0.2f, 0.2f);
    auto hsv = hsvColor(color);
    if (hsv.x + 20 > 360) {
        hsv.x = (hsv.x + 20 - 360);
    } else {
        hsv.x = hsv.x + 20;
    }
    color = rgbColor(hsv);
    return color;
}