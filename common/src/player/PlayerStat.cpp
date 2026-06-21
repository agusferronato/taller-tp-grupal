#include "Formulas.h"
#include "PlayerStats.h"

#include <limits>

uint32_t PlayerStats::getGold() const { return gold; }
uint32_t PlayerStats::getExperience() const { return experience; }
uint32_t PlayerStats::getLevel() const { return level; }
uint32_t PlayerStats::getHp() const { return health; }
uint32_t PlayerStats::getMaxHp() const { return maxHealth; }
uint32_t PlayerStats::getMana() const { return mana; }
uint32_t PlayerStats::getMaxMana() const { return maxMana; }
uint32_t PlayerStats::getStrength() const { return strength; }
uint32_t PlayerStats::getAgility() const { return agility; }
uint32_t PlayerStats::getConstitution() const { return constitution; }
uint32_t PlayerStats::getIntelligence() const { return intelligence; }
void PlayerStats::setHealth(uint32_t hp) {
  health = hp > maxHealth ? maxHealth : hp;
}
void PlayerStats::setMana(uint32_t mana) {
  this->mana = mana > maxMana ? maxMana : mana;
}

PlayerStats::PlayerStats(const PlayerStatsInfo &info)
    : gold(info.gold), experience(info.experience), level(info.level),
      health(info.health), mana(info.mana) {
  initStats(info.race, info.playerClass);
}

PlayerStats::PlayerStats(Race race, PlayerClass playerClass) {
  gold = 0;
  experience = 0;
  level = 1;
  initStats(race, playerClass);
  health = maxHealth;
  mana = maxMana;
}

void PlayerStats::updateStats(uint32_t hp, uint32_t maxHp, uint32_t mana,
                              uint32_t maxMana, uint32_t gold, uint32_t level,
                              uint32_t experience) {
  this->health = hp;
  this->maxHealth = maxHp;
  this->mana = mana;
  this->maxMana = maxMana;
  this->gold = gold;
  this->level = level;
  this->experience = experience;
}

void PlayerStats::addMana(uint32_t amount) {
  uint32_t newMana = mana + amount;
  if (newMana > maxMana)
    newMana = maxMana;
  mana = newMana;
}

void PlayerStats::initStats(Race race_, PlayerClass playerClass_) {
  race = race_;
  playerClass = playerClass_;
  auto [strengthRace, agilityRace, constitutionRace, intelligenceRace] =
      Formulas::getRaceStats(race);
  auto [strengthClass, agilityClass, constitutionClass, intelligenceClass] =
      Formulas::getPlayerClassStats(playerClass);

  strength = strengthRace + strengthClass;
  intelligence = intelligenceRace + intelligenceClass;
  agility = agilityRace + agilityClass;
  constitution = constitutionRace + constitutionClass;

  maxHealth = Formulas::calcularVidaMax(constitution, race, playerClass, level);
  maxMana = Formulas::calcularManaMax(intelligence, race, playerClass, level);
}

bool PlayerStats::addExperience(uint32_t xp) {
  experience += xp;
  uint32_t limite = Formulas::calcularLimiteXP(level);
  if (experience >= limite) {
    experience -= limite;
    level++;
    maxHealth =
        Formulas::calcularVidaMax(constitution, race, playerClass, level);
    maxMana = Formulas::calcularManaMax(intelligence, race, playerClass, level);
    health = maxHealth;
    mana = maxMana;
    return true;
  }
  return false;
}

void PlayerStats::setLevel(uint32_t newLevel) {
  level = newLevel < 1 ? 1 : newLevel;
  experience = 0;
  maxHealth = Formulas::calcularVidaMax(constitution, race, playerClass, level);
  maxMana = Formulas::calcularManaMax(intelligence, race, playerClass, level);
  health = maxHealth;
  mana = maxMana;
}

void PlayerStats::setGold(uint32_t amount) {
  uint32_t max = Formulas::calcularOroMax(level);
  max *= 1.5; // 50% de exceso permitido
  gold = amount > max ? max : amount;
}

void PlayerStats::takeDamage(uint32_t damage) {
  health = damage > health ? 0 : health - damage;
}

void PlayerStats::earnGold(uint32_t amount) {
  uint64_t newGold = static_cast<uint64_t>(gold) + amount;

  // Si da mas que el maximo permitido, se setea al maximo permitido
  if (newGold > std::numeric_limits<uint32_t>::max()) {
    setGold(std::numeric_limits<uint32_t>::max());
  } else {
    setGold(static_cast<uint32_t>(newGold));
  }
}

void PlayerStats::die() {
  health = 0;
  mana = 0;
}

void PlayerStats::heal(uint32_t amount) {
  uint32_t newHealth = health + amount;
  if (newHealth > maxHealth) {
    newHealth = maxHealth;
  }
  health = newHealth;
}
