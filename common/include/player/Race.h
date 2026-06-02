#ifndef RACE_H
#define RACE_H

#include <cstdint>
#include <string>

/*
  Race
  Representa las razas disponibles para los jugadores.
*/

enum class Race : uint8_t { Human, Elf, Dwarf, Gnome };

class RaceUtils {
public:
  static Race stringToRace(const std::string &raceStr);
  static std::string raceToString(Race race);
};

#endif // RACE_H
