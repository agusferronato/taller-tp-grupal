#ifndef CHEAT_COMMAND_H
#define CHEAT_COMMAND_H

#include "CheatType.h"
#include "Command.h"

class CheatCommand : public Command {
private:
  CheatType cheat;
  uint32_t arg;

public:
  explicit CheatCommand(CheatType cheat, uint32_t arg = 0);

  void execute(Game &game, uint32_t connectionId) override;
};

#endif
