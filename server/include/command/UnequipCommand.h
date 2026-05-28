#ifndef UNEQUIP_COMMAND_H
#define UNEQUIP_COMMAND_H

#include <cstdint>

#include "Command.h"

class UnequipCommand : public Command {
private:
  uint32_t playerId;
  uint8_t equipSlot;

public:
  UnequipCommand(uint32_t playerId, uint8_t equipSlot);

  void execute(Game &game) override;
};

#endif
