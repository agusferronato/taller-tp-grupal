#ifndef CHEAT_COMMAND_H
#define CHEAT_COMMAND_H

#include "CheatType.h"
#include "Command.h"
#include <string>

class CheatCommand : public Command {
private:
  CheatType cheat;
  uint32_t arg;
  std::string itemName;

public:
  explicit CheatCommand(CheatType cheat, uint32_t arg = 0,
                        std::string itemName = "");

  void execute(Game &game, uint32_t connectionId) override;
};

#endif
