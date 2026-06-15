#ifndef CLIENT_NPC_H
#define CLIENT_NPC_H

#include "Direction.h"
#include "EffectType.h"

class NPC {

private:
    int x, y;
    Direction direction;
    bool isMoving;
    mutable bool beingAttacked{false};
    mutable EffectType currentEffect{EffectType::NormalAttack};

public:

    NPC(int x, int y, Direction dir = Direction::Down);

    int get_x() const;
    int get_y() const;
    Direction getDirection() const { return direction; }
    bool getIsMoving() const { return isMoving; }

    void setCoordinates(int x, int y);
    void updateCoordinates(int x, int y, Direction direction);
    void stopMoving();

    bool isBeingAttackOrCured() const { return beingAttacked; }
    EffectType getEffect() const { return currentEffect; }
    void setBeingAttacked(bool v, EffectType effect = EffectType::NormalAttack) {
        beingAttacked = v;
        if (v) currentEffect = effect;
    }
    void stopAttackEffect() const {
        beingAttacked = false;
        currentEffect = EffectType::NormalAttack;
    }
};

#endif
