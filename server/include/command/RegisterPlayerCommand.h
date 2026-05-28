#ifndef REGISTER_PLAYER_COMMAND_H
#define REGISTER_PLAYER_COMMAND_H

#include <string>

#include "Command.h"

class RegisterPlayerCommand : public Command {
private:
  std::string name;
  std::string race;
  std::string playerClass;

public:
  RegisterPlayerCommand(std::string name, std::string race, std::string playerClass);

  void execute(Game &game) override;
};

#endif // REGISTER_PLAYER_COMMAND_H
