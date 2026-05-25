#ifndef REGISTER_PLAYER_COMMAND_H
#define REGISTER_PLAYER_COMMAND_H

#include <string>

#include "Command.h"

class RegisterPlayerCommand : public Command {
private:
  std::string name;

public:
  explicit RegisterPlayerCommand(std::string name);

  void execute(Game &game) override;
};

#endif // REGISTER_PLAYER_COMMAND_H
