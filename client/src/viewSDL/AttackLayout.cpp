#include "AttackLayout.h"

AttackLayout::AttackLayout(std::vector<SpriteData> &frames)
    : frames(frames) {}

SpriteData AttackLayout::getLayout(unsigned int iterator, int &frameOut) const {
    int count = frames.size();
    frameOut = static_cast<int>(iterator * 0.25) % count;
    return frames[frameOut];
}
