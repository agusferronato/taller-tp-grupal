#ifndef MEDITATE_REQUEST_DTO_H
#define MEDITATE_REQUEST_DTO_H

#include "protocol/ProtocolCodes.h"
#include <cstdint>

struct MeditateDTO {
private:
  static constexpr CommandOpCode opCode = CommandOpCode::Meditate;

public:
  CommandOpCode getCode() const { return opCode; }
  uint32_t playerId;
};

#endif
