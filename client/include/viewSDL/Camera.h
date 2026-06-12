#ifndef CAMERA_H
#define CAMERA_H

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>
#include <cmath>
#include <utility>

class Camera {
private:
  float x{0.0f}, y{0.0f};
  float zoom{1.0f};

  int viewportX{0};
  int viewportY{0};
  int viewportW;
  int viewportH;

public:
  Camera(int screenW, int screenH) : viewportW(screenW), viewportH(screenH) {}

  void setViewport(int x, int y, int w, int h) {
    viewportX = x;
    viewportY = y;
    viewportW = w;
    viewportH = h;
  }

  void follow(float targetX, float targetY, float targetW, float targetH) {
    x = targetX + targetW / 2.0f - viewportW / (2.0f * zoom);
    y = targetY + targetH / 2.0f - viewportH / (2.0f * zoom);
  }

  void setZoom(float newZoom) { zoom = newZoom; }

  float getX() const { return x; }
  float getY() const { return y; }
  int getViewportX() const { return viewportX; }
  int getViewportY() const { return viewportY; }

  SDL2pp::Rect getViewportRect() const {
    return SDL2pp::Rect(viewportX, viewportY, viewportW, viewportH);
  }

  bool isVisibleOnScreen(const SDL2pp::Rect &screenRect) const {
    SDL_Rect rect{screenRect.GetX(), screenRect.GetY(), screenRect.GetW(),
                  screenRect.GetH()};
    SDL_Rect viewport{viewportX, viewportY, viewportW, viewportH};
    return SDL_HasIntersection(&rect, &viewport) == SDL_TRUE;
  }

  bool isVisibleInWorld(float wx, float wy, int w, int h) const {
    SDL2pp::Rect screenRect = toScreen(wx, wy, w, h);
    return isVisibleOnScreen(screenRect);
  }

  std::pair<int, int> mouseToWorld(int mouseX, int mouseY) const {
    return std::make_pair(
        static_cast<int>((mouseX - viewportX) / zoom + x),
        static_cast<int>((mouseY - viewportY) / zoom + y));
  }

SDL2pp::Rect toScreen(float wx, float wy, int w, int h) const {
  int left = viewportX + static_cast<int>(std::round((wx - x) * zoom));
  int top = viewportY + static_cast<int>(std::round((wy - y) * zoom));
  int right = viewportX + static_cast<int>(std::round((wx + w - x) * zoom));
  int bottom = viewportY + static_cast<int>(std::round((wy + h - y) * zoom));

  return SDL2pp::Rect(left, top, right - left, bottom - top);
}
};

#endif
