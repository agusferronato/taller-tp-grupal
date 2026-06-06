#include "NPC.h"
#include "Character.h"

bool NPC::updatePosition(const Character& character) {
    int dx = character.getX() - x;
    int dy = character.getY() - y;

    if (abs(dx) > range || abs(dy) > range) {
        if (isMoving) {
            isMoving = false;
        }
        return false;
    }

    Direction dir;
    if (abs(dx) > abs(dy)) {
        dir = (dx > 0) ? Direction::Right : Direction::Left;
    } else {
        dir = (dy > 0) ? Direction::Down : Direction::Up;
    }

    direction = dir;

    switch (dir) {
        case Direction::Up:    y -= 1; break;
        case Direction::Down:  y += 1; break;
        case Direction::Left:  x -= 1; break;
        case Direction::Right: x += 1; break;
    }

    if (!isMoving) {
        isMoving = true;
    }

    return true;
}
