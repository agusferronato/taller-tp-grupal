#ifndef VALIDATE_LOGIN_COMMAND_H
#define VALIDATE_LOGIN_COMMAND_H

#include <string>

#include "Command.h"

class ValidateLoginCommand : public Command {
private:
  std::string playerName;

public:
  explicit ValidateLoginCommand(std::string playerName);

  void execute(Game &game, uint32_t connectionId) override;
};

#endif
