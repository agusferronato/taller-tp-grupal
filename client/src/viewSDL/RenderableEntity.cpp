#include "RenderableEntity.h"

bool RenderableEntity::rendered() { return wasRendered; }

bool RenderableEntity::hasPriority(int priority) {
  return this->priority == priority;
}

void RenderableEntity::clear() { wasRendered = false; }
