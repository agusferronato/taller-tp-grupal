#ifndef JOIN_CLAN_COMMAND_H
#define JOIN_CLAN_COMMAND_H

#include "Command.h"
#include <cstdint>
#include <string>

class JoinClanCommand : public Command {
private:
  uint32_t playerId;
  std::string clanName;

public:
  JoinClanCommand(uint32_t playerId, const std::string &clanName);

  void execute(Game &game, uint32_t connectionId) override;
};

#endif
