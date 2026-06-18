#include "HeadLayout.h"

HeadLayout::HeadLayout(std::map<Direction, SpriteData> &frames)
    : frames(frames) {}

SpriteData HeadLayout::getLayout(Direction direction) {
  return frames.at(direction);
}
