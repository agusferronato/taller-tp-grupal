#ifndef PRIVATE_MESSAGE_COMMAND_DTO_H
#define PRIVATE_MESSAGE_COMMAND_DTO_H

#include "protocol/ProtocolCodes.h"
#include <string>

struct PrivateMessageCommandDTO {
private:
  static constexpr ClientCommandOpCode opCode =
      ClientCommandOpCode::PrivateMessageCommand;

public:
  ClientCommandOpCode getCode() const { return opCode; }
  std::string target;
  std::string message;
};

#endif
