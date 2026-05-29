#ifndef EXIT_COMMAND_H
#define EXIT_COMMAND_H

#include <cstdint>

#include "Command.h"

class ExitCommand : public Command {
private:
  uint32_t playerId;

public:
  explicit ExitCommand(uint32_t playerId);

  void execute(Game &game, uint32_t connectionId) override;
};

#endif
