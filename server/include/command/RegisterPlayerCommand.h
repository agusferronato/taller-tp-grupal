#ifndef REGISTER_PLAYER_COMMAND_H
#define REGISTER_PLAYER_COMMAND_H

#include <cstdint>
#include <string>

#include "Command.h"

class RegisterPlayerCommand : public Command {
private:
  std::string playerName;

public:
  explicit RegisterPlayerCommand(const std::string &playerName);

  void execute(Game &game, uint32_t connectionId) override;
};

#endif // REGISTER_PLAYER_COMMAND_H
