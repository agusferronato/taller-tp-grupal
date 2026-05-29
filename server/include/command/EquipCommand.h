#ifndef EQUIP_COMMAND_H
#define EQUIP_COMMAND_H

#include <cstdint>

#include "Command.h"

class EquipCommand : public Command {
private:
  uint32_t playerId;
  uint8_t inventorySlot;

public:
  EquipCommand(uint32_t playerId, uint8_t inventorySlot);

  void execute(Game &game, uint32_t connectionId) override;
};

#endif
