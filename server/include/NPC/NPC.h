#ifndef NPC_H
#define NPC_H

#include <string>
#include "Colisionable.h"
#include "Direction.h"
#include "NPCType.h"
#include "Position.h"
#include <string>

class Character;

class NPC : public Colisionable {

protected:
    uint32_t id;
    Position gridPosition;
    int range{128};
    int attackCounter{0};
    int x, y;
    Direction direction{Direction::Down};
    bool isMoving{false};

public:
  explicit NPC(Position pos) : gridPosition(pos), x(0), y(0) {}

  virtual ~NPC() = default;

  virtual NPCType getType() = 0;

  void setId(uint32_t newId) { id = newId; }
  uint32_t getId() const { return id; }
  const Position &getPosition() const { return gridPosition; }
  int getX() const override { return x; }
  int getY() const override { return y; }

  virtual int getAncho() const = 0;
  virtual int getAlto() const = 0;
  virtual int getRange() const = 0;

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


    virtual uint32_t getDamage() = 0;
    bool collidesWith(Character &character);
    bool reachesAttackCounter();

    virtual int getAttackCounterMax() = 0;

    virtual std::string getName() = 0;

};

#endif
