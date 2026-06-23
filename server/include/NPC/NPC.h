#ifndef NPC_H
#define NPC_H

#include "Colisionable.h"
#include "Direction.h"
#include "NPCType.h"
#include "Position.h"
#include <cstdlib>
#include <string>

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
  NPC(Position pos, const NPCStats &stats);

  virtual ~NPC() = default;

  NPCType getType();

  void setId(uint32_t newId);
  uint32_t getId() const;
  const Position &getPosition() const;
  int getX() const override;
  int getY() const override;

  int getAncho() const override;
  int getAlto() const override;
  int getRange() const;

  Direction getDirection() const;
  bool getIsMoving() const;
  void setPixelPosition(int px, int py);
  void stop();

  bool colisionaCon(int targetX, int targetY, int targetAncho,
                    int targetAlto) const override;

  bool updatePosition(const Character &character);

  uint32_t getDamage();
  bool collidesWith(Character &character);

  uint32_t takeDamage(uint32_t damage);

  bool reachesAttackCounter();

  int getAttackCounterMax();

  std::string getName();

  uint32_t getHP() const;
  uint32_t getMaxHp() const;
  uint32_t getLevel() const;
  bool tryParry() const;
  ObjectDropped getDroppedObject() const;
};

#endif
