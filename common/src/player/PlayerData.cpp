#include "PlayerData.h"

#include <toml++/toml.hpp>

#include "PlayerClass.h"
#include "Race.h"

PlayerDataLoader &PlayerDataLoader::instance() {
  static PlayerDataLoader inst;
  return inst;
}

PlayerDataLoader::PlayerDataLoader() { loadFromToml(); }

const RaceData &PlayerDataLoader::getRaceData(Race race) const {
  return raceData.at(race);
}

const ClassData &PlayerDataLoader::getClassData(PlayerClass playerClass) const {
  return classData.at(playerClass);
}

void PlayerDataLoader::loadFromToml() {
  auto table = toml::parse_file("player_stats.toml");

  auto razaTable = table["raza"].as_table();
  for (auto &[key, value] : *razaTable) {
    Race race = RaceUtils::stringToRace(std::string(key.str()));
    auto &raceTbl = *value.as_table();
    RaceData data;
    data.vida = raceTbl["vida"].value_or(0.0);
    data.mana = raceTbl["mana"].value_or(0.0);
    data.recuperacion = raceTbl["recuperacion"].value_or(0.0);
    data.strengthRace = raceTbl["strengthRace"].value_or(0u);
    data.agilityRace = raceTbl["agilityRace"].value_or(0u);
    data.constitutionRace = raceTbl["constitutionRace"].value_or(0u);
    data.intelligenceRace = raceTbl["intelligenceRace"].value_or(0u);
    raceData[race] = data;
  }

  auto claseTable = table["clase"].as_table();
  for (auto &[key, value] : *claseTable) {
    PlayerClass pc =
        PlayerClassUtils::stringToPlayerClass(std::string(key.str()));
    auto &classTbl = *value.as_table();
    ClassData data;
    data.vida = classTbl["vida"].value_or(0.0);
    data.mana = classTbl["mana"].value_or(0.0);
    data.meditacion = classTbl["meditacion"].value_or(0.0);
    data.strengthClass = classTbl["strengthClass"].value_or(0u);
    data.agilityClass = classTbl["agilityClass"].value_or(0u);
    data.constitutionClass = classTbl["constitutionClass"].value_or(0u);
    data.intelligenceClass = classTbl["intelligenceClass"].value_or(0u);
    classData[pc] = data;
  }
}
