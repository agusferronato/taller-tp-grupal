#ifndef CHARACTER_H
#define CHARACTER_H

#include "Colisionable.h"
#include "Direction.h"
#include "Inventory.h"
#include "PlayerData.h"
#include "Race.h"
#include <cstdint>
#include <string>
#include <utility>

class Character : public Colisionable {
public:
  static constexpr int ANCHO = 32;
  static constexpr int ALTO = 32;

  uint32_t id;
  int x{0}, y{0};
  Direction direction{Direction::Down};
  bool moving{false};

  std::string name;
  std::string password;
  Race race;
  std::string playerClass;
  uint32_t level{1};
  uint32_t hp{100}, maxHp{100};
  uint32_t mana{0}, maxMana{0};
  uint32_t experience{0};
  uint32_t gold{0};
  uint32_t strength{};
  uint32_t agility{};
  uint32_t constitution{};
  uint32_t intelligence{};
  Inventory inventory;

  Character(uint32_t id, int x, int y, Direction dir);

  // Conversion methods
  PlayerData toPlayerData() const;
  void fromPlayerData(const PlayerData &data);

  // Movement logic
  std::pair<int, int> getTargetPosition(Direction dir) const;
  void setDirection(Direction dir) { direction = dir; }
  void setMoving(bool isMoving) { moving = isMoving; }

  // Initialize stats based on race and class
  void initializeStats(const Race &characterRace,
                       const std::string &characterClass);

  // Stats management
  void takeDamage(uint32_t damage);
  void heal(uint32_t amount);
  void gainExperience(uint32_t xp);
  void addGold(uint32_t amount);
  void spendGold(uint32_t amount);

  // Colisionable interface
  bool colisionaCon(int targetX, int targetY, int ancho,
                    int alto) const override;
  int getX() const override;
  int getY() const override;
  int getAncho() const override;
  int getAlto() const override;
};

#endif // CHARACTER_H
