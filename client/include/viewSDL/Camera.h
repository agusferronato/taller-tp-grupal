#ifndef CAMERA_H
#define CAMERA_H

#include <SDL2pp/SDL2pp.hh>
#include <utility>

class Camera {
private:
  float x{0.0f}, y{0.0f};

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
    x = targetX + targetW / 2.0f - viewportW / 2.0f;
    y = targetY + targetH / 2.0f - viewportH / 2.0f;
  }

  float getX() const { return x; }
  float getY() const { return y; }
  int getViewportX() const { return viewportX; }
  int getViewportY() const { return viewportY; }
  std::pair<int, int> mouseToWorld(int mouseX, int mouseY) const {
  return std::make_pair(
      static_cast<int>(mouseX - viewportX + x),
      static_cast<int>(mouseY - viewportY + y));
  }

  SDL2pp::Rect toScreen(float wx, float wy, int w, int h) const {
    return SDL2pp::Rect(viewportX + static_cast<int>(wx - x),
                        viewportY + static_cast<int>(wy - y), w, h);
  }
};

#endif
