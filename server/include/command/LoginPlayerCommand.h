#ifndef LOGIN_PLAYER_COMMAND_H
#define LOGIN_PLAYER_COMMAND_H

#include <string>

#include "Command.h"

class LoginPlayerCommand : public Command {
private:
  std::string name;

public:
  explicit LoginPlayerCommand(std::string name);

  void execute(Game &game, uint32_t connectionId) override;
};

#endif // LOGIN_PLAYER_COMMAND_H
