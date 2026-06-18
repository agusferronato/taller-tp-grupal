#ifndef HEAD_LAYOUT_H
#define HEAD_LAYOUT_H

#include "Direction.h"
#include "SpriteData.h"
#include <map>

class HeadLayout {
private:
  std::map<Direction, SpriteData> frames;

public:
  explicit HeadLayout(std::map<Direction, SpriteData> &frames);

  SpriteData getLayout(Direction direction);
};

#endif
