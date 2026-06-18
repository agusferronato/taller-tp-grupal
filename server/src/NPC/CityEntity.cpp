#include "CityEntity.h"
#include "Character.h"

CityEntity::CityEntity(Position pos) : gridPosition(pos), x(0), y(0) {}

void CityEntity::setId(uint32_t newId) { id = newId; }
uint32_t CityEntity::getId() const { return id; }
const Position& CityEntity::getPosition() const { return gridPosition; }
int CityEntity::getX() const { return x; }
int CityEntity::getY() const { return y; }
int CityEntity::getRange() { return range; }
Direction CityEntity::getDirection() const { return direction; }
bool CityEntity::getIsMoving() const { return isMoving; }
void CityEntity::setPixelPosition(int px, int py) { x = px; y = py; }
void CityEntity::stop() { isMoving = false; }

bool CityEntity::colisionaCon(int targetX, int targetY, int targetAncho,
                              int targetAlto) const {
    return !(targetX + targetAncho <= x ||
             targetX >= x + getAncho() ||
             targetY + targetAlto <= y ||
             targetY >= y + getAlto());
}

bool CityEntity::updatePosition(const Character& character) {
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
