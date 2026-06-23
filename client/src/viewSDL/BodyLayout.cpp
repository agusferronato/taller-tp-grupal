#include "BodyLayout.h"

BodyLayout::BodyLayout(std::map<Direction, std::vector<SpriteData>> &layout)
    : layout(layout) {}

SpriteData BodyLayout::getLayout(Direction direction, unsigned int iterator) {
  const auto &frames = layout.at(direction);
  int count = frames.size();
  int frame = static_cast<int>(iterator * 0.25) % count;
  return frames[frame];
}
