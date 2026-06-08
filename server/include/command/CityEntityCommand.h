#ifndef CITY_ENTITY_COMMAND_H
#define CITY_ENTITY_COMMAND_H

#include "Command.h"
#include <cstdint>

class CityEntityCommand : public Command {
private:
  uint32_t playerId;
  uint8_t type;
  int16_t arg;

public:
  CityEntityCommand(uint32_t playerId, uint8_t type, int16_t arg);
  void execute(Game &game, uint32_t connectionId) override;
};

#endif
