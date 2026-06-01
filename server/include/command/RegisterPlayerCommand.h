#ifndef REGISTER_PLAYER_COMMAND_H
#define REGISTER_PLAYER_COMMAND_H

#include <cstdint>
#include <string>

#include "Command.h"
#include "Race.h"

class RegisterPlayerCommand : public Command {
private:
  std::string name;
  Race race;
  std::string playerClass;

public:
  RegisterPlayerCommand(std::string name, Race race, std::string playerClass);

  void execute(Game &game, uint32_t connectionId) override;
};

#endif // REGISTER_PLAYER_COMMAND_H
