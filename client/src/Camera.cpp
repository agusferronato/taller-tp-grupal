#include "Camera.h"


Camera::Camera(float screenW, float screenH) : screenW(screenW), screenH(screenH) {}

void Camera::follow(float targetX, float targetY, float targetW, float targetH) {
    x = targetX + targetW / 2.0f - screenW / 2.0f;
    y = targetY + targetH / 2.0f - screenH / 2.0f;
}

SDL2pp::Rect Camera::toScreen(float wx, float wy, int w, int h) const {
    return SDL2pp::Rect(static_cast<int>(wx - x), static_cast<int>(wy - y), w,
                        h);
}
