#include "NPC.h"

NPC::NPC(int x, int y, Direction dir)
    : x(x), y(y), direction(dir), isMoving(false) {}

int NPC::get_x() const { return x; }
int NPC::get_y() const { return y; }
Direction NPC::getDirection() const { return direction; }
bool NPC::getIsMoving() const { return isMoving; }
bool NPC::isBeingAttackOrCured() const { return beingAttacked; }
EffectType NPC::getEffect() const { return currentEffect; }
void NPC::setBeingAttacked(bool v, EffectType effect) {
  beingAttacked = v;
  if (v)
    currentEffect = effect;
}
void NPC::stopAttackEffect() const {
  beingAttacked = false;
  currentEffect = EffectType::NormalAttack;
}

void NPC::setCoordinates(int x, int y) {
  this->x = x;
  this->y = y;
}

void NPC::updateCoordinates(int x, int y, Direction direction) {
  setCoordinates(x, y);
  isMoving = true;
  if (direction != this->direction)
    this->direction = direction;
}

void NPC::stopMoving() { isMoving = false; }
