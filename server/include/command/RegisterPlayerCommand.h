#ifndef REGISTER_PLAYER_COMMAND_H
#define REGISTER_PLAYER_COMMAND_H

#include <cstdint>
#include <string>

#include "Command.h"
#include "PlayerClass.h"
#include "Race.h"

class RegisterPlayerCommand : public Command {
private:
  std::string name;
  Race race;
  PlayerClass playerClass;

public:
  RegisterPlayerCommand(std::string name, Race race, PlayerClass playerClass);

  void execute(Game &game, uint32_t connectionId) override;
};

#endif // REGISTER_PLAYER_COMMAND_H
