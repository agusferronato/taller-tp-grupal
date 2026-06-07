#ifndef ATTACK_COMMAND_H
#define ATTACK_COMMAND_H

#include "Command.h"
#include <cstdint>

class AttackCommand : public Command {
private:
  uint32_t playerId;
  int16_t x;
  int16_t y;

public:
  AttackCommand(uint32_t playerId, int16_t x, int16_t y);
  void execute(Game &game, uint32_t connectionId) override;
};

#endif
