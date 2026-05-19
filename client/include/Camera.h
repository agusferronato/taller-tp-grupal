#ifndef CAMERA_H
#define CAMERA_H

#include <SDL2pp/SDL2pp.hh>

class Camera {

private:
  float x{0.0f}, y{0.0f};
  int screenW, screenH;

public:
  Camera(float screenW, float screenH) : screenW(screenW), screenH(screenH) {}

  void follow(float targetX, float targetY, float targetW, float targetH) {
    x = targetX + targetW / 2.0f - screenW / 2.0f;
    y = targetY + targetH / 2.0f - screenH / 2.0f;
  }

  SDL2pp::Rect toScreen(float wx, float wy, int w, int h) const {
    return SDL2pp::Rect(static_cast<int>(wx - x), static_cast<int>(wy - y), w,
                        h);
  }
};

#endif
