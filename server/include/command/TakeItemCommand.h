#ifndef TAKE_ITEM_COMMAND_H
#define TAKE_ITEM_COMMAND_H

#include <cstdint>

#include "Command.h"

class TakeItemCommand : public Command {
private:
  uint32_t playerId;

public:
  explicit TakeItemCommand(uint32_t playerId);

  void execute(Game &game, uint32_t connectionId) override;
};

#endif
