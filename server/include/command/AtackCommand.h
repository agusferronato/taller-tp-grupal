#ifndef ATACK_COMMAND_H
#define ATACK_COMMAND_H

#include "Command.h"
#include <cstdint>

class AtackCommand : public Command {
private:
  uint32_t playerId;
  int16_t x;
  int16_t y;

public:
  AtackCommand(uint32_t playerId, int16_t x, int16_t y);
  void execute(Game &game, uint32_t connectionId) override;
};

#endif
