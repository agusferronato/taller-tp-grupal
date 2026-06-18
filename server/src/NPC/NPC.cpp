#include "NPC.h"
#include "Character.h"
#include "Formulas.h"

NPC::NPC(Position pos, const NPCStats& stats)
    : gridPosition(pos),
      range(stats.range), attackCounterMax(stats.attackCounterMax),
      x(0), y(0),
      hp(stats.hp), maxHp(stats.hp), level(stats.level),
      agility(stats.agility), damage(stats.damage),
      ancho(stats.ancho), alto(stats.alto),
      type(stats.type), name(stats.name) {}

bool NPC::colisionaCon(int targetX, int targetY, int targetAncho,
                       int targetAlto) const {
    return !(targetX + targetAncho <= x ||
             targetX >= x + getAncho() ||
             targetY + targetAlto <= y ||
             targetY >= y + getAlto());
} 

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

bool NPC::collidesWith(Character& character) {
    return colisionaCon(
        character.getX(),
        character.getY(),
        character.getAncho(),
        character.getAlto()
    );
}

bool NPC::reachesAttackCounter()
{
    if (attackCounter < getAttackCounterMax()) {
        attackCounter++;
        return false;
    }

    attackCounter = 0;
    return true;
}

uint32_t NPC::takeDamage(uint32_t damage) {
    if (damage >= hp) {
        hp = 0;
    } else {
        hp -= damage;
    }
    return damage;
}

NPCType NPC::getType() { return type; }

void NPC::setId(uint32_t newId) { id = newId; }
uint32_t NPC::getId() const { return id; }
const Position& NPC::getPosition() const { return gridPosition; }
int NPC::getX() const { return x; }
int NPC::getY() const { return y; }
int NPC::getAncho() const { return ancho; }
int NPC::getAlto() const { return alto; }
int NPC::getRange() const { return range; }
Direction NPC::getDirection() const { return direction; }
bool NPC::getIsMoving() const { return isMoving; }
void NPC::setPixelPosition(int px, int py) { x = px; y = py; }
void NPC::stop() { isMoving = false; }
uint32_t NPC::getDamage() { return damage; }
int NPC::getAttackCounterMax() { return attackCounterMax; }
std::string NPC::getName() { return name; }
uint32_t NPC::getHP() const { return hp; }
uint32_t NPC::getMaxHp() const { return maxHp; }
uint32_t NPC::getLevel() const { return level; }

bool NPC::tryParry() const {
    return Formulas::calcularEsquivo(agility, std::rand() % 2);
}

ObjectDropped NPC::getDroppedObject() const {
    int roll = std::rand() % 100;

    if (roll < 80) {
        return {ObjectDroppedType::None, 0};
    } else if (roll < 88) {
        double factor = 0.01 + (std::rand() % 100) / 100.0 * 0.19;
        uint32_t gold = static_cast<uint32_t>(factor * maxHp);
        return {ObjectDroppedType::Gold, gold};
    } else if (roll < 89) {
        uint8_t potionId = (std::rand() % 2 == 0) ? 18 : 19;
        return {ObjectDroppedType::Item, potionId};
    } else if (roll < 90) {
        uint8_t itemId = 1 + std::rand() % 17;
        return {ObjectDroppedType::Item, itemId};
    }

    return {ObjectDroppedType::None, 0};
}
