#ifndef BAN_CLAN_PLAYER_COMMAND_H
#define BAN_CLAN_PLAYER_COMMAND_H

#include "Command.h"
#include <cstdint>
#include <string>

class BanClanPlayerCommand : public Command {
private:
  uint32_t founderId;
  std::string playerName;

public:
  BanClanPlayerCommand(uint32_t founderId, const std::string &playerName);

  void execute(Game &game, uint32_t connectionId) override;
};

#endif
