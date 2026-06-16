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

  uint32_t getId() const { return id; }
  const std::string &getName() const { return player.getName(); }
  Direction getDirection() const { return player.getDirection(); }
  Race getRace() const { return player.getRace(); }
  PlayerClass getPlayerClass() const { return player.getPlayerClass(); }
  uint32_t getHp() const { return player.getHp(); }
  uint32_t getMaxHp() const { return player.getMaxHp(); }
  uint32_t getMana() const { return player.getMana(); }
  uint32_t getMaxMana() const { return player.getMaxMana(); }
  uint32_t getLevel() const { return player.getLevel(); }
  uint32_t getExperience() const { return player.getExperience(); }
  uint32_t getGold() const { return player.getGold(); }
  uint32_t getIntelligence() const { return player.getIntelligence(); }

  // conversion para red o persistencia
  PlayerData toPlayerData() const;
  PlayerInfoDTO toPlayerInfo(uint32_t playerId) const;
  PlayerInfoEventDTO toPlayerInfoEvent() const;
  PlayerAppearedEventDTO toPlayerAppeared() const;
  PlayerMovedEventDTO toPlayerMoved() const;

  bool isMoving() const { return player.isMoving(); }
  void setDirection(Direction dir);
  void stop();
  std::pair<int, int> getTargetPosition(Direction dir) const;
  std::pair<int, int> getTargetPosition(Direction dir, uint32_t speed) const;
  std::pair<int, int> getTargetPosition() const;
  std::pair<int, int> getTargetPosition(uint32_t speed) const;
  void move(int newX, int newY) { player.move(newX, newY); }

  // Inventory (delegates to Player)
  bool addItem(uint8_t itemId) { return player.addItem(itemId); }
  bool equipItem(uint8_t slot) { return player.equipItem(slot); }
  bool unequipSlot(EquipSlot slot) { return player.unequipSlot(slot); }
  bool removeItem(uint8_t slot) { return player.removeItem(slot); }
  bool hasItem(uint8_t itemId) const;
  bool hasMoney(uint16_t amount) const;
  void removeItemById(uint8_t itemId);
  void resurrect();

  Inventory &getInventory() { return player.getInventory(); }
  const Inventory &getInventory() const { return player.getInventory(); }

  // Inventory getters
  std::array<uint8_t, MAX_INVENTORY_SLOTS> getInventoryItems() const {
    return player.getInventoryItems();
  }
  const Weapon &getEquippedWeapon() const { return player.getEquippedWeapon(); }
  const Armor &getEquippedArmor() const { return player.getEquippedArmor(); }
  const Helmet &getEquippedHelmet() const { return player.getEquippedHelmet(); }
  const Shield &getEquippedShield() const { return player.getEquippedShield(); }

  // Stats management (delegates to Player)
  uint32_t takeDamage(uint32_t damage);
  void heal(uint32_t amount);
  void addMana(uint32_t amount) { player.addMana(amount); }
  bool useMana(uint32_t amount) { return player.useMana(amount); }
  void gainExperience(uint32_t xp);
  void setLevel(uint32_t level) { player.setLevel(level); }
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

  bool isNewbie() const { return player.getLevel() < 13; }
  bool isDead() const { return player.isDead(); }
  uint32_t getDamage() const;
  bool tryParry() const;
  bool assertAttackDistance(int16_t targetX, int16_t targetY) const;
  uint32_t dropGoldOnDeath();
  std::vector<uint8_t> die();
  void restore();

  uint32_t getClanId() const { return player.getClanId(); }
  bool hasClan() const { return player.hasClan(); }
  void joinClan(uint32_t clanId) { player.joinClan(clanId); }
  void leaveClan() { player.leaveClan(); }

  bool isMeditating() const { return meditating; }
  void setMeditating(bool value) { meditating = value; }

private:
  uint32_t id;
  Player player;

  std::optional<uint8_t> timeSinceLastHit;
  std::optional<uint8_t> timeSinceLastManaConsume;
  bool meditating{false};
};

#endif // CHARACTER_H
