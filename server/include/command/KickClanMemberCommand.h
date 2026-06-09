#ifndef KICK_CLAN_MEMBER_COMMAND_H
#define KICK_CLAN_MEMBER_COMMAND_H

#include "Command.h"
#include <cstdint>
#include <string>

class KickClanMemberCommand : public Command {
private:
  uint32_t founderId;
  std::string playerName;

public:
  KickClanMemberCommand(uint32_t founderId, const std::string &playerName);

  void execute(Game &game, uint32_t connectionId) override;
};

#endif
