#ifndef ATTACK_COMMAND_DTO_H
#define ATTACK_COMMAND_DTO_H

#include "protocol/ProtocolCodes.h"
#include <cstdint>

struct AttackCommandDTO {
private:
  static constexpr ClientCommandOpCode opCode =
      ClientCommandOpCode::AttackCommand;

public:
  ClientCommandOpCode getCode() const { return opCode; }

  uint32_t playerId;
  int16_t x;
  int16_t y;
};

#endif
