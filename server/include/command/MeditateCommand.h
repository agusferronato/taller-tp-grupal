#ifndef MEDITATE_COMMAND_H
#define MEDITATE_COMMAND_H

#include <cstdint>

#include "Command.h"

class MeditateCommand : public Command {
public:
  explicit MeditateCommand(uint32_t playerId);

  void execute(Game &game, uint32_t connectionId) override;

private:
  uint32_t playerId;
};

#endif
