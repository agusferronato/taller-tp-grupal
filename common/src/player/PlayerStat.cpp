#include "Formulas.h"
#include "PlayerStats.h"

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

void PlayerStats::initStats(Race race, PlayerClass playerClass) {
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
