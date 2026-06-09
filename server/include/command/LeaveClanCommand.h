#ifndef LEAVE_CLAN_COMMAND_H
#define LEAVE_CLAN_COMMAND_H

#include "Command.h"
#include <cstdint>

class LeaveClanCommand : public Command {
private:
  uint32_t playerId;

public:
  explicit LeaveClanCommand(uint32_t playerId);

  void execute(Game &game, uint32_t connectionId) override;
};

#endif
