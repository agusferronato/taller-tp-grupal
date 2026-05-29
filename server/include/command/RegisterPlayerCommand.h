#ifndef REGISTER_PLAYER_COMMAND_H
#define REGISTER_PLAYER_COMMAND_H

#include <cstdint>

#include "Command.h"

class RegisterPlayerCommand : public Command {
public:
  void execute(Game &game, uint32_t connectionId) override;
};

#endif // REGISTER_PLAYER_COMMAND_H
