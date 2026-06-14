#ifndef ACCEPT_CLAN_REQUEST_COMMAND_H
#define ACCEPT_CLAN_REQUEST_COMMAND_H

#include "Command.h"
#include <cstdint>
#include <string>

class AcceptClanRequestCommand : public Command {
private:
  uint32_t founderId;
  std::string playerName;

public:
  AcceptClanRequestCommand(uint32_t founderId, const std::string &playerName);

  void execute(Game &game, uint32_t connectionId) override;
};

#endif
