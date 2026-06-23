#ifndef REJECT_CLAN_REQUEST_COMMAND_H
#define REJECT_CLAN_REQUEST_COMMAND_H

#include "Command.h"
#include <cstdint>
#include <string>

class RejectClanRequestCommand : public Command {
private:
  uint32_t founderId;
  std::string playerName;

public:
  RejectClanRequestCommand(uint32_t founderId, const std::string &playerName);

  void execute(Game &game, uint32_t connectionId) override;
};

#endif
