#ifndef HEAD_LAYOUT_H
#define HEAD_LAYOUT_H

#include <map>
#include "Direction.h"
#include "SpriteData.h"

class HeadLayout {
private:
    std::map<Direction, SpriteData> frames;

public:
    explicit HeadLayout(std::map<Direction, SpriteData>& frames) : frames(frames) {}

    SpriteData getLayout(Direction direction) {
        return frames.at(direction);
    }
};

#endif
