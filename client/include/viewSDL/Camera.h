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
  Camera(int screenW, int screenH);

  void setViewport(int x, int y, int w, int h);

  void follow(float targetX, float targetY, float targetW, float targetH);

  void setZoom(float newZoom);

  float getX() const;
  float getY() const;
  int getViewportX() const;
  int getViewportY() const;

  SDL2pp::Rect getViewportRect() const;

  bool isVisibleOnScreen(const SDL2pp::Rect &screenRect) const;

  bool isVisibleInWorld(float wx, float wy, int w, int h) const;

  std::pair<int, int> mouseToWorld(int mouseX, int mouseY) const;

  SDL2pp::Rect toScreen(float wx, float wy, int w, int h) const;
};

#endif
