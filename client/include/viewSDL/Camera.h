#ifndef CAMERA_H
#define CAMERA_H

#include <SDL2pp/SDL2pp.hh>

class Camera {

private:
  float x{0.0f}, y{0.0f};
  int screenW, screenH;

public:
  Camera(float screenW, float screenH);

  void follow(float targetX, float targetY, float targetW, float targetH);

  SDL2pp::Rect toScreen(float wx, float wy, int w, int h) const;
};

#endif
