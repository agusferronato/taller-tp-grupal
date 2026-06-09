#ifndef EXIT_COMMAND_H
#define EXIT_COMMAND_H

#include <cstdint>

#include "Command.h"

class ExitCommand : public Command {
public:
  ExitCommand() = default;

  void execute(Game &game, uint32_t connectionId) override;
};

#endif
