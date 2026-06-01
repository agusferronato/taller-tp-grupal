#include "Character.h"
#include "Formulas.h"
#include <algorithm>

Character::Character(uint32_t id, int x, int y, Direction dir)
    : id(id), x(x), y(y), direction(dir) {}

PlayerData Character::toPlayerData() const {
  PlayerData data{};
  data.setName(name);
  data.setPassword(password);
  data.setRace(RaceUtils::raceToString(race));
  data.setPlayerClass(playerClass);
  data.x = x;
  data.y = y;
  data.direction = static_cast<uint8_t>(direction);
  data.level = level;
  data.hp = hp;
  data.maxHp = maxHp;
  data.mana = mana;
  data.maxMana = maxMana;
  data.experience = experience;
  data.gold = gold;
  data.strength = strength;
  data.agility = agility;
  data.constitution = constitution;
  data.intelligence = intelligence;
  data.inventory = inventory.getItems();
  data.equippedWeapon = inventory.getWeapon();
  data.equippedArmor = inventory.getArmor();
  data.equippedHelmet = inventory.getHelmet();
  data.equippedShield = inventory.getShield();
  return data;
}

void Character::fromPlayerData(const PlayerData &data) {
  name = data.name;
  password = data.password;
  race = RaceUtils::stringToRace(data.race);
  playerClass = data.playerClass;
  x = data.x;
  y = data.y;
  direction = static_cast<Direction>(data.direction);
  level = data.level;
  hp = data.hp;
  maxHp = data.maxHp;
  mana = data.mana;
  maxMana = data.maxMana;
  experience = data.experience;
  gold = data.gold;
  strength = data.strength;
  agility = data.agility;
  constitution = data.constitution;
  intelligence = data.intelligence;
  inventory.setItems(data.inventory);
  inventory.setWeapon(data.equippedWeapon);
  inventory.setArmor(data.equippedArmor);
  inventory.setHelmet(data.equippedHelmet);
  inventory.setShield(data.equippedShield);
}

static std::string lowercase(const std::string &s) {
  std::string result = s;
  auto toLower = [](unsigned char c) { return std::tolower(c); };
  std::transform(result.begin(), result.end(), result.begin(), toLower);
  return result;
}

static void initPlayerStats(Character &player, const Race race,
                            const std::string &playerClass) {
  struct BaseStats {
    uint32_t strength, agility, constitution, intelligence;
  };

  auto getRaceStats = [](const Race &race) -> BaseStats {
    switch (race) {
    case Race::Human:
      return {10, 10, 10, 10};
    case Race::Elf:
      return {6, 13, 5, 16};
    case Race::Dwarf:
      return {13, 4, 16, 7};
    case Race::Gnome:
      return {7, 6, 14, 13};
    }
    throw std::invalid_argument("Invalid race");
  };

  auto getClassStats = [](const std::string &c) -> BaseStats {
    std::string lc = lowercase(c);
    if (lc == "mago")
      return {3, 5, 5, 15};
    if (lc == "clerigo")
      return {7, 7, 9, 10};
    if (lc == "paladin")
      return {10, 6, 10, 8};
    return {10, 8, 10, 3};
  };

  auto raceStats = getRaceStats(race);
  auto classStats = getClassStats(playerClass);
  player.strength = raceStats.strength + classStats.strength;
  player.agility = raceStats.agility + classStats.agility;
  player.constitution = raceStats.constitution + classStats.constitution;
  player.intelligence = raceStats.intelligence + classStats.intelligence;
}

bool Character::colisionaCon(int targetX, int targetY, int ancho,
                             int alto) const {
  return !(targetX + ancho <= x || targetX >= x + ANCHO ||
           targetY + alto <= y || targetY >= y + ALTO);
}

int Character::getX() const { return x; }

int Character::getY() const { return y; }

int Character::getAncho() const { return ANCHO; }

int Character::getAlto() const { return ALTO; }

std::pair<int, int> Character::getTargetPosition(Direction dir) const {
  int targetX = x;
  int targetY = y;
  switch (dir) {
  case Direction::Up:
    targetY -= 1;
    break;
  case Direction::Down:
    targetY += 1;
    break;
  case Direction::Left:
    targetX -= 1;
    break;
  case Direction::Right:
    targetX += 1;
    break;
  }
  return {targetX, targetY};
}

void Character::initializeStats(const Race &characterRace,
                                const std::string &characterClass) {
  initPlayerStats(*this, characterRace, characterClass);
  maxHp = Formulas::calcularVidaMax(constitution, characterRace, characterClass,
                                    level);
  hp = maxHp;
  maxMana = Formulas::calcularManaMax(intelligence, characterRace,
                                      characterClass, level);
  mana = maxMana;
}

void Character::takeDamage(uint32_t damage) {
  if (damage >= hp) {
    hp = 0;
  } else {
    hp -= damage;
  }
}

void Character::heal(uint32_t amount) { hp = std::min(hp + amount, maxHp); }

void Character::gainExperience(uint32_t xp) { experience += xp; }

void Character::addGold(uint32_t amount) { gold += amount; }

void Character::spendGold(uint32_t amount) {
  if (amount <= gold) {
    gold -= amount;
  }
}
