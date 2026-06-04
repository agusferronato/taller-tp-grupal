#ifndef REGISTER_PLAYER_COMMAND_DTO_H
#define REGISTER_PLAYER_COMMAND_DTO_H

#include "PlayerClass.h"
#include "Race.h"
#include "protocol/ProtocolCodes.h"
#include <string>

struct RegisterPlayerCommandDTO {
private:
  static constexpr ClientCommandOpCode opCode =
      ClientCommandOpCode::RegisterPlayerCommand;

public:
  ClientCommandOpCode getCode() const { return opCode; }
  std::string name;
  Race race;
  PlayerClass playerClass;
};

#endif
