#ifndef BODY_LAYOUT_H
#define BODY_LAYOUT_H

#include "Direction.h"
#include "SpriteData.h"
#include <map>
#include <vector>

class BodyLayout {
private:
  std::map<Direction, std::vector<SpriteData>> layout;

public:
  explicit BodyLayout(std::map<Direction, std::vector<SpriteData>> &layout);

  SpriteData getLayout(Direction direction, unsigned int iterator);
};

#endif 
