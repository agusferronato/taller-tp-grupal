#include "Camera.h"

Camera::Camera(int screenW, int screenH)
    : viewportW(screenW), viewportH(screenH) {}

void Camera::setViewport(int x, int y, int w, int h) {
  viewportX = x;
  viewportY = y;
  viewportW = w;
  viewportH = h;
}

void Camera::follow(float targetX, float targetY, float targetW, float targetH) {
  x = targetX + targetW / 2.0f - viewportW / (2.0f * zoom);
  y = targetY + targetH / 2.0f - viewportH / (2.0f * zoom);
}

void Camera::setZoom(float newZoom) { zoom = newZoom; }

float Camera::getX() const { return x; }
float Camera::getY() const { return y; }
int Camera::getViewportX() const { return viewportX; }
int Camera::getViewportY() const { return viewportY; }

SDL2pp::Rect Camera::getViewportRect() const {
  return SDL2pp::Rect(viewportX, viewportY, viewportW, viewportH);
}

bool Camera::isVisibleOnScreen(const SDL2pp::Rect &screenRect) const {
  SDL_Rect rect{screenRect.GetX(), screenRect.GetY(), screenRect.GetW(),
                screenRect.GetH()};
  SDL_Rect viewport{viewportX, viewportY, viewportW, viewportH};
  return SDL_HasIntersection(&rect, &viewport) == SDL_TRUE;
}

bool Camera::isVisibleInWorld(float wx, float wy, int w, int h) const {
  SDL2pp::Rect screenRect = toScreen(wx, wy, w, h);
  return isVisibleOnScreen(screenRect);
}

std::pair<int, int> Camera::mouseToWorld(int mouseX, int mouseY) const {
  return std::make_pair(
      static_cast<int>((mouseX - viewportX) / zoom + x),
      static_cast<int>((mouseY - viewportY) / zoom + y));
}

SDL2pp::Rect Camera::toScreen(float wx, float wy, int w, int h) const {
  int left = viewportX + static_cast<int>(std::round((wx - x) * zoom));
  int top = viewportY + static_cast<int>(std::round((wy - y) * zoom));
  int right = viewportX + static_cast<int>(std::round((wx + w - x) * zoom));
  int bottom = viewportY + static_cast<int>(std::round((wy + h - y) * zoom));

  return SDL2pp::Rect(left, top, right - left, bottom - top);
}
