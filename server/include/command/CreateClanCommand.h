#ifndef CREATE_CLAN_COMMAND_H
#define CREATE_CLAN_COMMAND_H

#include "Command.h"
#include <cstdint>
#include <string>

class CreateClanCommand : public Command {
private:
  uint32_t playerId;
  std::string clanName;

public:
  CreateClanCommand(uint32_t playerId, const std::string &clanName);

  void execute(Game &game, uint32_t connectionId) override;
};

#endif
