#ifndef BODY_LAYOUT_H
#define BODY_LAYOUT_H

#include <map>
#include <vector>
#include "Direction.h"
#include "SpriteData.h"

class BodyLayout {
private:
    std::map<Direction, std::vector<SpriteData>> layout;

public:
    explicit BodyLayout(std::map<Direction, std::vector<SpriteData>>& layout) : layout(layout) {}

    SpriteData getLayout(Direction direction, unsigned int iterator) {
        auto& frames = layout.at(direction);
        int count = frames.size();
        int frame = static_cast<int>(iterator * 0.25) % count;
        return frames[frame];
    }
};

#endif
