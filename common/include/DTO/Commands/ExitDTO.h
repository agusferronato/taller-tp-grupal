#ifndef EXIT_DTO_H
#define EXIT_DTO_H

#include "protocol/ProtocolCodes.h"
#include <cstdint>

struct ExitDTO {
private:
  static constexpr CommandOpCode opCode = CommandOpCode::Exit;

public:
  CommandOpCode getCode() const { return opCode; }
  uint32_t playerId;
};

#endif
