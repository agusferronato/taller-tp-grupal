#ifndef RACE_H
#define RACE_H

#include <cstdint>
#include <stdexcept>
#include <string>

/*
  Race
  Representa las razas disponibles para los jugadores.
*/

enum class Race : uint8_t { Human, Elf, Dwarf, Gnome };

class RaceUtils {
public:
  static Race stringToRace(const std::string &raceStr) {
    if (raceStr == "Humano") {
      return Race::Human;
    } else if (raceStr == "Elfo") {
      return Race::Elf;
    } else if (raceStr == "Enano") {
      return Race::Dwarf;
    } else if (raceStr == "Gnomo") {
      return Race::Gnome;
    }
    throw std::invalid_argument("Invalid race string: " + raceStr);
  }

  static std::string raceToString(Race race) {
    switch (race) {
    case Race::Human:
      return "Humano";
    case Race::Elf:
      return "Elfo";
    case Race::Dwarf:
      return "Enano";
    case Race::Gnome:
      return "Gnomo";
    default:
      throw std::invalid_argument("Invalid race");
    }
  }
};

#endif // RACE_H
