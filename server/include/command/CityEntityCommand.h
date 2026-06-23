#ifndef CITY_ENTITY_COMMAND_H
#define CITY_ENTITY_COMMAND_H

#include "Command.h"
#include <cstdint>
#include <string>

class CityEntityCommand : public Command {
private:
  uint32_t playerId;
  uint8_t type;
  std::string arg;

public:
  CityEntityCommand(uint32_t playerId, uint8_t type, std::string arg);
  void execute(Game &game, uint32_t connectionId) override;
};

#endif
