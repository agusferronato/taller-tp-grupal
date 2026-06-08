#ifndef BAN_CLAN_PLAYER_COMMAND_DTO_H
#define BAN_CLAN_PLAYER_COMMAND_DTO_H

#include "protocol/ProtocolCodes.h"
#include <cstdint>
#include <string>

struct BanClanPlayerCommandDTO {
private:
  static constexpr ClientCommandOpCode opCode =
      ClientCommandOpCode::BanClanPlayerCommand;

public:
  ClientCommandOpCode getCode() const { return opCode; }
  uint32_t founderId;
  std::string playerName;
};

#endif
