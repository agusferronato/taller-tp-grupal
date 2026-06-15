#ifndef NPC_H
#define NPC_H

#include <cstdlib>
#include <string>
#include "Colisionable.h"
#include "Direction.h"
#include "NPCType.h"
#include "Position.h"

class Character;

enum class ObjectDroppedType : uint8_t { None, Gold, Potion, Item };

struct ObjectDropped {
    ObjectDroppedType type = ObjectDroppedType::None;
    uint32_t value = 0;
};

class NPC : public Colisionable {

public:
    struct NPCStats {
        NPCType type;
        std::string name;
        uint32_t level;
        uint32_t hp;
        uint32_t agility;
        uint32_t damage;
        int attackCounterMax;
        int ancho;
        int alto;
        int range;
    };

protected:
    uint32_t id;
    Position gridPosition;
    int range;
    int attackCounterMax;
    int attackCounter{0};
    int x, y;
    Direction direction{Direction::Down};
    bool isMoving{false};
    uint32_t hp;
    uint32_t maxHp;
    uint32_t level;
    uint32_t agility;
    uint32_t damage;
    int ancho;
    int alto;
    NPCType type;
    std::string name;

public:
  NPC(Position pos, const NPCStats& stats);

  virtual ~NPC() = default;

  NPCType getType() { return type; }

  void setId(uint32_t newId) { id = newId; }
  uint32_t getId() const { return id; }
  const Position& getPosition() const { return gridPosition; }
  int getX() const override { return x; }
  int getY() const override { return y; }

  int getAncho() const override { return ancho; }
  int getAlto() const override { return alto; }
  int getRange() const { return range; }

  Direction getDirection() const { return direction; }
  bool getIsMoving() const { return isMoving; }
  void setPixelPosition(int px, int py) {
    x = px;
    y = py;
  }
  void stop() { isMoving = false; }

  bool colisionaCon(int targetX, int targetY, int targetAncho,
                    int targetAlto) const override;

  bool updatePosition(const Character &character);

    uint32_t getDamage() { return damage; }
    bool collidesWith(Character &character);

    uint32_t takeDamage(uint32_t damage);

    bool reachesAttackCounter();

    int getAttackCounterMax() { return attackCounterMax; }

    std::string getName() { return name; }

    uint32_t getHP() const { return hp; }
    uint32_t getMaxHp() const { return maxHp; }
    uint32_t getLevel() const { return level; }
    bool tryParry() const;
    ObjectDropped getDroppedObject() const;

};

#endif
