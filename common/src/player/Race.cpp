#include "Race.h"
#include <stdexcept>

Race RaceUtils::stringToRace(const std::string &raceStr) {
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

std::string RaceUtils::raceToString(Race race) {
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
