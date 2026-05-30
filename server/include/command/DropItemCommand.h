#ifndef DROP_ITEM_COMMAND_H
#define DROP_ITEM_COMMAND_H

#include <cstdint>

#include "Command.h"

class DropItemCommand : public Command {
private:
  uint32_t playerId;
  uint8_t inventorySlot;

public:
  DropItemCommand(uint32_t playerId, uint8_t inventorySlot);

  void execute(Game &game, uint32_t connectionId) override;
};

#endif
