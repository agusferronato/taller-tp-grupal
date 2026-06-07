#ifndef ATTACK_LAYOUT_H
#define ATTACK_LAYOUT_H

#include "SpriteData.h"
#include <vector>

class AttackLayout {
private:
    std::vector<SpriteData> frames;
public:
    explicit AttackLayout(std::vector<SpriteData> &frames);
    SpriteData getLayout(unsigned int iterator, int &frameOut) const;
    int size() const { return frames.size(); }
};

#endif
