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
    Direction getDirection() const;
    bool getIsMoving() const;

    void setCoordinates(int x, int y);
    void updateCoordinates(int x, int y, Direction direction);
    void stopMoving();

    bool isBeingAttackOrCured() const;
    EffectType getEffect() const;
    void setBeingAttacked(bool v, EffectType effect = EffectType::NormalAttack);
    void stopAttackEffect() const;
};

#endif
