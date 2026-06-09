#ifndef CREATE_CLAN_COMMAND_DTO_H
#define CREATE_CLAN_COMMAND_DTO_H

#include "protocol/ProtocolCodes.h"
#include <cstdint>
#include <string>

struct CreateClanCommandDTO {
private:
  static constexpr ClientCommandOpCode opCode =
      ClientCommandOpCode::CreateClanCommand;

public:
  ClientCommandOpCode getCode() const { return opCode; }
  uint32_t playerId;
  std::string clanName;
};

#endif
