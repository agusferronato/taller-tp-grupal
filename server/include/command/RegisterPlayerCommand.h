#ifndef REGISTER_PLAYER_COMMAND_H
#define REGISTER_PLAYER_COMMAND_H

#include <cstdint>
#include <string>

#include "Command.h"

class RegisterPlayerCommand : public Command {
private:
  std::string playerName;
  std::string race;
  std::string playerClass;

public:
  RegisterPlayerCommand(std::string playerName, std::string race, std::string playerClass);

  void execute(Game &game, uint32_t connectionId) override;
};

#endif // REGISTER_PLAYER_COMMAND_H
