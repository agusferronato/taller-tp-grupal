#ifndef PLAYER_DATA_H
#define PLAYER_DATA_H

#include <cstdint>
#include <unordered_map>

#include "PlayerClass.h"
#include "Race.h"

struct RaceData {
  double vida;
  double mana;
  double recuperacion;
  uint32_t strengthRace;
  uint32_t agilityRace;
  uint32_t constitutionRace;
  uint32_t intelligenceRace;
};

struct ClassData {
  double vida;
  double mana;
  double meditacion;
  uint32_t strengthClass;
  uint32_t agilityClass;
  uint32_t constitutionClass;
  uint32_t intelligenceClass;
};

class PlayerDataLoader {
public:
  static PlayerDataLoader &instance();

  const RaceData &getRaceData(Race race) const;
  const ClassData &getClassData(PlayerClass playerClass) const;

private:
  PlayerDataLoader();
  ~PlayerDataLoader() = default;
  PlayerDataLoader(const PlayerDataLoader &) = delete;
  PlayerDataLoader &operator=(const PlayerDataLoader &) = delete;

  void loadFromToml();

  std::unordered_map<Race, RaceData> raceData;
  std::unordered_map<PlayerClass, ClassData> classData;
};

#endif
