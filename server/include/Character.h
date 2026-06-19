#ifndef CHARACTER_H
#define CHARACTER_H

#include "Armor.h"
#include "City.h"
#include "Colisionable.h"
#include "Direction.h"
#include "Helmet.h"
#include "Player.h"
#include "PlayerAppearedEventDTO.h"
#include "PlayerClass.h"
#include "PlayerData.h"
#include "PlayerInfoEventDTO.h"
#include "PlayerListEventDTO.h"
#include "PlayerMovedEventDTO.h"
#include "Race.h"
#include "Shield.h"
#include "Weapon.h"
#include <cstdint>
#include <list>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

class Character : public Colisionable {
public:
  static constexpr int ANCHO = 32;
  static constexpr int ALTO = 32;

  // Register constructor: new character
  Character(uint32_t id, std::string name, Race race, PlayerClass playerClass,
            int x, int y, Direction dir);

  // Login constructor: restore from saved data
  Character(uint32_t id, const PlayerData &data);

  uint32_t getId() const;
  const std::string &getName() const;
  Direction getDirection() const;
  Race getRace() const;
  PlayerClass getPlayerClass() const;
  uint32_t getHp() const;
  uint32_t getMaxHp() const;
  uint32_t getMana() const;
  uint32_t getMaxMana() const;
  uint32_t getLevel() const;
  uint32_t getExperience() const;
  uint32_t getGold() const;
  uint32_t getIntelligence() const;

  // conversion para red o persistencia
  PlayerData toPlayerData() const;
  PlayerInfoDTO toPlayerInfo(uint32_t playerId) const;
  PlayerInfoEventDTO toPlayerInfoEvent() const;
  PlayerAppearedEventDTO toPlayerAppeared() const;
  PlayerMovedEventDTO toPlayerMoved() const;

  bool isMoving() const;
  void setDirection(Direction dir);
  void stop();
  std::pair<int, int> getTargetPosition(Direction dir) const;
  std::pair<int, int> getTargetPosition(Direction dir, uint32_t speed) const;
  std::pair<int, int> getTargetPosition() const;
  std::pair<int, int> getTargetPosition(uint32_t speed) const;
  void move(int newX, int newY);

  // Inventory (delegates to Player)
  bool addItem(uint8_t itemId);
  bool equipItem(uint8_t slot);
  bool unequipSlot(EquipSlot slot);
  bool removeItem(uint8_t slot);
  bool hasItem(uint8_t itemId) const;
  bool hasMoney(uint16_t amount) const;
  void removeItemById(uint8_t itemId);
  void resurrect();

  Inventory &getInventory();
  const Inventory &getInventory() const;

  // Inventory getters
  std::array<uint8_t, MAX_INVENTORY_SLOTS> getInventoryItems() const;
  const Weapon &getEquippedWeapon() const;
  const Armor &getEquippedArmor() const;
  const Helmet &getEquippedHelmet() const;
  const Shield &getEquippedShield() const;

  // Stats management (delegates to Player)
  uint32_t takeDamage(uint32_t damage);
  void heal(uint32_t amount);
  void addMana(uint32_t amount);
  bool useMana(uint32_t amount);
  void gainExperience(uint32_t xp);
  void setLevel(uint32_t level);
  void addGold(uint32_t amount);
  void spendGold(uint32_t amount);

  // Colisionable interface
  bool colisionaCon(int targetX, int targetY, int ancho,
                    int alto) const override;

  bool isInCity(std::list<City> &cities, int gridSize, int maxSize);
  int getX() const override;
  int getY() const override;
  int getAncho() const override;
  int getAlto() const override;

  bool isNewbie() const;
  bool isDead() const;
  uint32_t getDamage() const;
  bool tryParry();
  bool assertAttackDistance(int16_t targetX, int16_t targetY) const;
  uint32_t dropGoldOnDeath();
  std::vector<uint8_t> die();
  void restore();

  uint32_t getClanId() const;
  bool hasClan() const;
  void joinClan(uint32_t clanId);
  void leaveClan();

  bool isMeditating();
  void startMeditating();
  void stopMeditating();

private:
  uint32_t id;
  Player player;

  std::optional<uint8_t> timeSinceLastHit;
  std::optional<uint8_t> timeSinceLastManaConsume;
  std::optional<uint8_t> timeSinceMeditating;
};

#endif // CHARACTER_H
