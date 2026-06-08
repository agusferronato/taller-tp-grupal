#ifndef REVIEW_CLAN_COMMAND_H
#define REVIEW_CLAN_COMMAND_H

#include "Command.h"
#include <cstdint>

class ReviewClanCommand : public Command {
private:
  uint32_t playerId;

public:
  explicit ReviewClanCommand(uint32_t playerId);

  void execute(Game &game, uint32_t connectionId) override;
};

#endif
