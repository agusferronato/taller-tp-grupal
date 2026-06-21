#ifndef RENDERABLE_ENTITY_H
#define RENDERABLE_ENTITY_H

#include "Camera.h"
#include <SDL2pp/SDL2pp.hh>

class RenderableEntity {

private:
  int priority{2};

protected:
  bool wasRendered{false};

public:
  virtual void render(SDL2pp::Renderer &renderer, Camera &camera,
                      unsigned int it) = 0;

  virtual int get_x() = 0;
  virtual int get_y() = 0;
  virtual int get_h() = 0;

  bool rendered();

  bool hasPriority(int priority);

  void clear();

  virtual ~RenderableEntity() = default;
};

#endif
